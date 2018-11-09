#include "haske.hpp"

void haske::reguser(eosio::name account, std::string first_name, std::string last_name, std::string country, std::string mobile, std::string biometrics){

}

void haske::placecollat(eosio::name borrower, std::string collateral_name, std::string collateral_description, std::string ipfs_collateral_url, std::vector<eosio::name> guarantors){

}

void haske::guarantee(eosio::name guarantor, uint64_t collateral_id, uint64_t level_od_confidence){

}

void haske::placelendable(eosio::name lender, eosio::asset amount, std::string terms){

}

EOSIO_DISPATCH(haske, (reguser)(placecollat)(guarantee)(placelendable))