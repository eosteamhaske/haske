#pragma once

// Avoid "using namsepace <>" in headers. Be explicit here
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/action.hpp>
// #include <eosiolib/action.h>
#include <eosiolib/time.hpp>
#include <eosiolib/multi_index.hpp>

#define EOS_SYMBOL = symbol("EOS", 4)

#define MIN_GUARANTORS = 3;

using eosio::action;
// using namespace eosio;

// class [[eosio::contract]] haske : public eosio::contract {
CONTRACT haske : public eosio::contract
{

  public:
    using contract::contract;

    haske(eosio::name self, eosio::name code, eosio::datastream<const char *> ds) : eosio::contract(self, code, ds), _users(self, code.value), _agents(self, code.value), _collaterals(self, code.value), _lendables(self, code.value) {}

    ACTION reguser(eosio::name username, std::string first_name, std::string last_name, std::string country, std::string mobile);

    ACTION addagent(eosio::name agent, std::string officialname, std::string address, std::string telephone, std::string email, std::string country);

    ACTION kyc(eosio::name agent, eosio::name user);

    ACTION placecollat(eosio::name borrower, std::string collateral_name, std::string collateral_description, std::string ipfs_collateral_url, std::vector<eosio::name> guarantors);

    ACTION guarantee(eosio::name guarantor, uint64_t collateral_id, uint64_t level_od_confidence);

    ACTION placelendable(eosio::name lender, eosio::asset amount, std::string terms);

    ACTION fund(eosio::name lender, eosio::name borrower, eosio::asset amount, uint64_t lendable_id);

    ACTION blacklist(eosio::name authority, std::vector<eosio::name> accounts);

  

  private:
    TABLE user
    {
        uint64_t id;
        eosio::name username;
        std::string first_name;
        std::string last_name;
        std::string country;
        std::string mobile;
        std::string ipfs_identity; // Path to ID documents. Should be encrypted
        bool is_kycd;
        eosio::name kyc_agent;

        auto primary_key() const { return username.value; }
        uint64_t by_id() const { return id; }
    };

    typedef eosio::multi_index<"users"_n, user,
                               eosio::indexed_by<"id"_n, eosio::const_mem_fun<user, uint64_t, &user::by_id>>>
        users_table;

    TABLE kyc_agent
    {
        uint64_t id;
        eosio::name agentname;
        std::string officialname;
        std::string address;
        std::string telephone;
        std::string email;
        std::string country;

        uint64_t primary_key() const{return agentname.value;}
    };

    typedef eosio::multi_index<"agents"_n, kyc_agent> agents_table;
    TABLE collateral
    {
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
                               eosio::indexed_by<"users"_n, eosio::const_mem_fun<collateral, uint64_t, &collateral::by_borrower>>>
        collaterals_table;

    TABLE lendable
    {
        uint64_t id;
        eosio::asset amount;
        eosio::name lender;
        std::string terms;

        uint64_t primary_key() const { return id; }

        uint64_t by_lender() const { return lender.value; }
    };

    typedef eosio::multi_index<"lendables"_n, lendable,
                               eosio::indexed_by<"lenders"_n, eosio::const_mem_fun<lendable, uint64_t, &lendable::by_lender>>>
        lendables_table;

    users_table _users;
    agents_table _agents;
    collaterals_table _collaterals;
    lendables_table _lendables;
};
