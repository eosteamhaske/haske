#pragma once

// Avoid "using namsepace <>" in headers. Be explicit here
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/time.hpp>
#include <eosiolib/multi_index.hpp>

#define EOS_SYMBOL = symbol("EOS", 4)

#define MIN_GUARANTORS = 3;

using eosio::action;

class [[eosio::contract]] haske : public eosio::contract {

    public:
        using contract::contract;

        haske(eosio::name self, eosio::name code, eosio::datastream<const char *> ds) : eosio::contract(self, code, ds){}


        [[eosio::action]]
        void reguser(eosio::name uesername, std::string first_name, std::string last_name, std::string country, std::string mobile, std::string biometrics);

        [[eosio::action]]
        void placecollat(eosio::name borrower, std::string collateral_name, std::string collateral_description, std::string ipfs_collateral_url, std::vector<eosio::name> guarantors);

        [[eosio::action]]
        void guarantee(eosio::name guarantor, uint64_t collateral_id, uint64_t level_od_confidence);

        [[eosio::action]]
        void placelendable(eosio::name lender, eosio::asset amount, std::string terms);

        [[eosio::action]]
        void fund(eosio::name lender, eosio::name borrower, eosio::asset amount, uint64_t lendable_id);

    private:

      struct [[eosio::table("users")]] user {
        uint64_t id;
        eosio::name uesername;
        std::string first_name;
        std::string last_name;
        std::string country;
        std::string mobile;
        std::string biometrics;

        auto primary_key() const { return uesername.value; }
        uint64_t by_id() const { return id; }
      } ;

      typedef eosio::multi_index<"users"_n, user, 
              eosio::indexed_by<"id"_n, eosio::const_mem_fun<user, uint64_t, &user::by_id>>> users_table;

    struct [[eosio::table("collaterals")]] collateral {
        uint64_t id;
        eosio::name user;
        std::string collateral_name;
        std::string description;
        std::string ipfs_path;
        std::vector<eosio::name> guarantors;
        bool is_guaranteed;

        auto primary_key() const { return id; }
        uint64_t by_borrower() const { return user.value; }
    };

    typedef eosio::multi_index<"collaterals"_n, collateral,
            eosio::indexed_by<"users"_n, eosio::const_mem_fun<collateral, uint64_t, &collateral::by_borrower>>> collaterals_table;


    struct [[eosio::table("lendables")]] lendable {
        uint64_t id;
        // TODO
    };
};





