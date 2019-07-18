#include

using namespace eosio;

CONTRACT addressbook: public contract {
public:
using contract::contract;


ACTION upsert() {}

ACTION insert(name user, std::string first_name, std::string last_name, uint32_t age) {
require_auth(user);

address_index forInsert(get_self(), get_self().value);
auto itr = forInsert.find(user.value);

check(itr == forInsert.end(), "already exists");

forInsert.emplace(user, [&](auto& row) {
row.user = user;
row.first_name = first_name;
row.last_name = last_name;
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

private:
struct [[eosio::table]] person {
name user;
std::string first_name;
std::string last_name;
uint32_t age;

uint64_t primary_key() const { return user.value; }
};

typedef multi_index<"people"_n, person> address_index;
};