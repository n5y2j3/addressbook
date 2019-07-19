#include<eosio/eosio.hpp>

using namespace eosio;

CONTRACT addressbook: public contract {
public:
using contract::contract;

ACTION findage(uint64_t age) {
    address_index addresses(get_self(), get_self().value);
    auto forSecondary = addresses.get_index<"byage"_n>();

    auto itr = forSecondary.require_find(age, "no age");

    print(itr->user, " ", itr->age);
}

ACTION insert(name user, uint64_t age) {
    require_auth(user);

    address_index forInsert(get_self(), get_self().value);
    auto itr = forInsert.find(user.value);

    check(itr == forInsert.end(), "already exists");

    forInsert.emplace(user, [&](auto& row) {
        row.user = user;
        row.age = age;
    });

print("insert success");
}

ACTION erase(name user) {
require_auth(user);

address_index forErase(get_self(), get_self().value);
auto itr = forErase.require_find(user.value, "no account");
forErase.erase(itr);

print("erase success");
}

ACTION eraseall() {
    require_auth(get_self());

    address_index forEraseAll(get_self(), get_self().value);
    auto itr = forEraseAll.begin();
    while(itr != forEraseAll.end()) { itr = forEraseAll.erase(itr); }
}

private:
    struct [[eosio::table]] person {
        name user;
        uint32_t age;

        uint64_t primary_key() const { return user.value; }
        uint64_t by_age() const { return age; }
};

typedef multi_index<"peopletwo"_n, person,
indexed_by<"byage"_n, const_mem_fun<person, uint64_t, &person::by_age>>> address_index;
};