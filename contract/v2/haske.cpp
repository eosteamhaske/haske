#include "haske.hpp"

ACTION haske::reguser(eosio::name account, std::string first_name, std::string last_name, std::string country, std::string mobile)
{
    require_auth(account);
    eosio_assert(first_name.size() > 0, "the first name should not be blank");
    eosio_assert(last_name.size() > 0, "the last name should not be blank");
    eosio_assert(country.size() > 0, "the country should not be blank");
    eosio_assert(mobile.size() > 0, "the mobile number is vital and should be included");

    _users.emplace(get_self(), [&](auto &u) {
        u.id = _users.available_primary_key();
        u.username = account;
        u.first_name = first_name;
        u.last_name = last_name;
        u.country = country;
        u.mobile = mobile;
    });
}

ACTION haske::addagent(eosio::name agent, std::string officialname, std::string address, std::string telephone, std::string email, std::string country)
{
    // Add KYC agents.

    require_auth(eosio::permission_level(_self, eosio::name("addagent")));

    _agents.emplace(get_self(), [&](auto &a) {
        a.id = _agents.available_primary_key();
        a.officialname = officialname;
        a.address = address;
        a.telephone = telephone;
        a.email = email;
        a.country = country;
    });
}

ACTION haske::kyc(eosio::name agent, eosio::name user)
{
    // implement KYC.
    // Once the KYC agent has performed a proper confirmation, they can mark an account as having been compliant

    require_auth(agent);
    auto usr = _users.find(user.value);

    if (usr != _users.end())
    {
        eosio_assert(usr->is_kycd == false,
                     "this account is already KYC'd");
        _users.modify(*usr, get_self(), [&](auto &u) {
            u.is_kycd = true;
            u.kyc_agent = agent;
        });
    }
}
ACTION haske::placecollat(eosio::name borrower, std::string collateral_name, std::string collateral_description, std::string ipfs_collateral_url, std::vector<eosio::name> guarantors)
{
    // User places collaterals
    

}

ACTION haske::guarantee(eosio::name guarantor, uint64_t collateral_id, uint64_t level_od_confidence)
{
    // Guarantors
}

ACTION haske::placelendable(eosio::name lender, eosio::asset amount, std::string terms)
{
    // Add lendables.
}

ACTION haske::blacklist(eosio::name authority, std::vector<eosio::name> accounts)
{
    // Flag / Blacklist bad actors on the platform
}

EOSIO_DISPATCH(haske, (reguser)(placecollat)(guarantee)(placelendable))