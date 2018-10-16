/*
 @authors Team Haske => (Meshach Ishaya, Munachi Ogueku, Hakeem Orewole)

 @event EOS - Africa Virtual Hackathon (OCTOBER 15-18, 2018)


 ****This contract facilitates complete decentralized lending on EOSIO****

 -borrowers register with biometric data and may put a collateral on the blockchain,

 -lenders decide which collaterals the credit scoring of the borrowers accounts they can work with and

 -loans are issued and paid via token : Haske Token (HSK)

 -lending duration expiration has collaterals automatically transfered to lenders

 - a credit scoring algorithm for borrowers
 */

#include <eosiolib/time.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using namespace eosio;

class haske : public eosio::contract {

public:
  using contract::contract;
  haske(account_name self): contract(self) {};

  /*
    This action manages user accounts: registrations and biometrics and updates on the haske protocol
  */
  [[eosio::action]]
  void registry(account_name user, std::string first_name, std::string last_name, std::string country, std::string mobile, std::string biometrics) {
    require_auth( user );

    //Assert params before runing table operations
    eosio_assert(first_name != " " || first_name.size() <= 64, "First name cannot be null and can only be 64 characters");
    eosio_assert(last_name != " " || last_name.size() <= 64, "Last name cannot be null and can only be 64 characters");
    eosio_assert(country != " " || country.size() <= 32, "Country cannot be null and can only be 32 characters");
    eosio_assert(mobile != " " || mobile.size() <= 15, "First name cannot be null and can only be 15 characters");
    eosio_assert(biometrics != " ", "Biometric field cannot be null");

    user_index users(_self, user);

    auto itr = users.find( user );
    if( itr == users.end() )
    {
      //Create user if not found
      users.emplace(user, [&]( auto& user_data ) {
       user_data.account = user;
       user_data.first_name = first_name;
       user_data.last_name = last_name;
       user_data.country = country;
       user_data.mobile = mobile;
       user_data.credit_score = 0;
       user_data.biometrics=biometrics;
      });
      info(user, "account has been successfully registered");
    }
    else {
      //Update user if found
      std::string changes;
      users.modify(itr, user, [&]( auto& user_data ) {
        user_data.account = user;
        user_data.first_name = first_name;
        user_data.last_name = last_name;
        user_data.country = country;
        user_data.mobile = mobile;
      });
      info(user, "account has been successfully updated");
    }
  }

  /*
   This action adds a collateral associated with a user account intending to borrow
  */
  [[eosio::action]]
  void collateral(account_name borrower, std::string description, std::string asset_type) {

    //Require an authentication from a borrower's account
    require_auth( borrower );

    //Assert params
    eosio_assert(description != " " || description.size() > 0, "You need to write a description about your collateral" );
    eosio_assert(asset_type != " " , "Your collateral must belong to an asset type" );

    transact_index transacts(_self, borrower);


    //Add the collateral data/meta-data to the blockchain
      transacts.emplace(borrower, [&]( auto& borrowing ) {
       borrowing.id = transacts.available_primary_key();
       borrowing.borrowing_account = borrower;
       borrowing.description = description;
       borrowing.asset_type = asset_type;
       borrowing.status = 0;
      });
      info(borrower, ", your collateral has been placed on the blockchain");
  }

  /*
   This action matches a lending account to a borrowing account based on the lenders review
  */
  [[eosio::action]]
  void lend(account_name lender, account_name borrower, asset haske_token, uint64_t _duration_in_minutes, uint64_t lend_id) {
    //Requires authentication from the lenders account
    require_auth( lender );

    transact_index transacts(_self, _self);

    auto iterator = transacts.find( lend_id );

    eosio_assert(iterator != transacts.end(), "This transaction has no borrower");
    eosio_assert(iterator->status != 1, "This loan is active");

    //update the transact table , set status ,time duration and loan amount of Haske-tokens
      transacts.modify(iterator, lender, [&]( auto& lending ) {
       lending.lending_account = lender;
       lending.status = 1;
       lending.haske_token = haske_token;
       lending.duration = eosio::time_point_sec(_duration_in_minutes * 60);
       lending.time_confirmed = eosio::time_point_sec(now());
      });

      //transfer lend amount to borrower in Haske-tokens
      transfer(lender, borrower, haske_token,"transfer successfull");
      info(lender, ", lend successfull");
  }

  /*
   This action allows a borrowing account return the borrowed Haske-tokens and redeem a collateral within lending duration
  */
  [[eosio::action]]
  void payback(account_name borrower, uint64_t lend_id, asset haske_token) {
    //Requires authentication from the lenders account
    require_auth( borrower );
    transact_index transacts(_self, _self);

    auto iterator = transacts.find( lend_id );
    eosio_assert(iterator != transacts.end(), "This transaction does not exist");
    eosio_assert(iterator->borrowing_account != borrower, "This account is not authorized to carry out this transaction");
    eosio_assert(haske_token.amount > iterator->haske_token.amount, "Insufficient Haske tokens to pay back the loan");
    eosio_assert(iterator->lend_default != 0 , "This collateral has been transfered to the lender");

    //Update transact table: set status = 1 and lend_default = 1
    if(eosio::time_point_sec(now()) > eosio::time_point_sec(iterator->duration) && iterator->lending_grace == 0)
    {
      transacts.modify(iterator, borrower, [&]( auto& borrowing ) {
       borrowing.lend_default = 1;
      });
      info(borrower, ",This collateral has been transfered to the lender");
      creditscore(borrower);
    }else{
      //transfer lend amount back to lender in Haske-tokens

      transfer(iterator->borrowing_account, iterator->lending_account, iterator->haske_token,"Loan paid");
      creditscore(borrower, 0.616);

      transacts.modify(iterator, borrower, [&]( auto& borrowing ) {
       borrowing.status = 0;
       borrowing.lend_default=0;
      });

      info(borrower, ",Loan successfully paid");
    }
  }

  /*
   This action allows a lending account to transfer back a collateral to a defaulting borrowing account
  */
  [[eosio::action]]
  void release(account_name lender, uint64_t lend_id)
  {
    require_auth(lender);
    transact_index transacts(_self, _self);

    auto iterator = transacts.find( lend_id );
    eosio_assert(iterator != transacts.end(), "This transaction does not exist");
    eosio_assert(iterator->lend_default != 0,"Cannot return a non-defaulting loan");
    eosio_assert(iterator->lending_account != lender, "This account is not authorized to carry out this transaction");


    transacts.modify(iterator, lender, [&](auto borrowing){
      borrowing.lending_grace=1;
    });
    info(lender, "Collateral returned successfully");
  }

  /*
   Assigns a credit score to a borrowing account:
   an arithmetic system is currently used . A more systematic algorithm can be implemented to best reflect an accounts loan credit score
  */
  [[eosio::action]]
  void creditscore(account_name borrower, double sentinel=1.616){ //sentinel = 1.616, for defaulting payments, 0.616 for non-defaaulting payments
    require_auth(_self);
    user_index users(_self, _self);
    auto iterator = users.find(borrower);
    eosio_assert(iterator != users.end(), "Account does not exist");

    users.modify(iterator, _self, [&](auto user){
      user.credit_score=user.credit_score/sentinel;
    });
  }

  /*
   Create haske-tokens and determine the maximum supply
  */
  void create( account_name issuer, asset maximum_supply )
  {
      require_auth( _self );

      auto sym = maximum_supply.symbol;
      eosio_assert( sym.is_valid(), "invalid symbol name" );
      eosio_assert( maximum_supply.is_valid(), "invalid supply");
      eosio_assert( maximum_supply.amount > 0, "max-supply must be positive");

      stats statstable( _self, sym.name() );
      auto existing = statstable.find( sym.name() );
      eosio_assert( existing == statstable.end(), "token with symbol already exists" );

      statstable.emplace( _self, [&]( auto& s ) {
         s.supply.symbol = maximum_supply.symbol;
         s.max_supply    = maximum_supply;
         s.issuer        = issuer;
      });
  }

  /*
   Issue haske-tokens to an account
  */
   void issue( account_name to, asset quantity, std::string memo )
  {
      //Only this contract may issue tokens
      require_auth(_self);

      auto sym = quantity.symbol;
      eosio_assert( sym.is_valid(), "invalid symbol name" );
      eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

      auto sym_name = sym.name();
      stats statstable( _self, sym_name );
      auto existing = statstable.find( sym_name );
      eosio_assert( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
      const auto& st = *existing;

      require_auth( st.issuer );
      eosio_assert( quantity.is_valid(), "invalid quantity" );
      eosio_assert( quantity.amount > 0, "must issue positive quantity" );

      eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
      eosio_assert( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

      statstable.modify( st, 0, [&]( auto& s ) {
         s.supply += quantity;
      });

      add_balance( st.issuer, quantity, st.issuer );

      if( to != st.issuer ) {
         SEND_INLINE_ACTION( *this, transfer, {st.issuer,N(active)}, {st.issuer, to, quantity, memo} );
      }
  }

  /*
  transfer haske-tokens between two accounts
  */
  void transfer( account_name from, account_name to, asset quantity, std::string  memo)
  {
      eosio_assert( from != to, "cannot transfer to self" );
      require_auth( from );
      eosio_assert( is_account( to ), "to account does not exist");
      auto sym = quantity.symbol.name();
      stats statstable( _self, sym );
      const auto& st = statstable.get( sym );

      require_recipient( from );
      require_recipient( to );

      eosio_assert( quantity.is_valid(), "invalid quantity" );
      eosio_assert( quantity.amount > 0, "must transfer positive quantity" );
      eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
      eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );


      sub_balance( from, quantity );
      add_balance( to, quantity, from );
  }

  /*
    This action is called by the summary method
  */
  [[eosio::action]]
  void notify(account_name user, std::string msg)
  {
  require_auth(get_self());
  require_recipient(user);
 }

  //Get total supply of Haske-tokens in the system
  asset get_supply( symbol_name sym )const
  {
     stats statstable( _self, sym );
     const auto& st = statstable.get( sym );
     return st.supply;
  }

  //Get balance of Haske-tokens for an account
  asset get_balance( account_name owner, symbol_name sym )const
  {
     accounts accountstable( _self, owner );
     const auto& ac = accountstable.get( sym );
     return ac.balance;
  }

private:


  //Safety implementations to subtract haske-tokens from the balance of an account
  void sub_balance( account_name owner, asset value ) {
     accounts from_acnts( _self, owner );

     const auto& from = from_acnts.get( value.symbol.name(), "no balance object found" );
     eosio_assert( from.balance.amount >= value.amount, "overdrawn balance" );


     if( from.balance.amount == value.amount ) {
        from_acnts.erase( from );
     } else {
        from_acnts.modify( from, owner, [&]( auto& a ) {
            a.balance -= value;
        });
     }
  }

  //Safety implementations to add haske-tokens to the balance of an account
  void add_balance( account_name owner, asset value, account_name ram_payer )
  {
     accounts to_acnts( _self, owner );
     auto to = to_acnts.find( value.symbol.name() );
     if( to == to_acnts.end() ) {
        to_acnts.emplace( ram_payer, [&]( auto& a ){
          a.balance = value;
        });
     } else {
        to_acnts.modify( to, 0, [&]( auto& a ) {
          a.balance += value;
        });
     }
  }

  //Send info to accounts after an action is exceuted successfuly
  void info(account_name user, std::string message){
    action(
      permission_level{get_self(),N(active)},
      get_self(),
      N(notify),
      std::make_tuple(user, name{user}.to_string() + " " + message)
    ).send();
  }

  //user table struct
  struct [[eosio::table]] user {
    account_name account;
    std::string  first_name;
    std::string  last_name;
    std::string  country;
    std::string  mobile;
    std::string  biometrics;
    uint64_t     credit_score;

    uint64_t primary_key() const { return account; }
    EOSLIB_SERIALIZE(user, (account)(first_name)(last_name)(country)(mobile)(credit_score))
  };

  //transact table struct
  struct [[eosio::table]] transact{
    uint64_t                 id;
    account_name             borrowing_account;
    account_name             lending_account;
    std::string              asset_type;
    asset                    haske_token;
    bool                     status = 0;
    bool                     lend_default = 0;
    bool                     lending_grace = 0;
    eosio::time_point_sec    duration;
    eosio::time_point_sec    time_confirmed;
    std::string              description;

    uint64_t primary_key() const { return id; }

    EOSLIB_SERIALIZE( transact, (id)(borrowing_account)(lending_account)(asset_type)(haske_token)(status)(lend_default)(lending_grace)(duration)(time_confirmed)(description) )
  };

  //account table struct
  struct [[eosio::table]] account {
     asset    balance;

     uint64_t primary_key()const { return balance.symbol.name(); }
  };

 //currency_stats table struct
  struct [[eosio::table]] currency_stats {
     asset          supply;
     asset          max_supply;
     account_name   issuer;

     uint64_t primary_key()const { return supply.symbol.name(); }
  };

 //typedef decalaration for accounts and currency_stats
  typedef eosio::multi_index<N(accounts), account> accounts;
  typedef eosio::multi_index<N(stat), currency_stats> stats;

  //typdef declaration for user and transact
  typedef eosio::multi_index<N(user), user> user_index;
  typedef eosio::multi_index<N(transact), transact> transact_index;

};

EOSIO_ABI( haske, (registry)(collateral)(release)(lend)(payback)(creditscore)(create)(transfer)(issue) )
