#include <vector>
#include <string>

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
using namespace eosio;

using std::vector;
using std::string;
using std::find;


static constexpr symbol TLOS_SYM = symbol("TLOS", 4);
// static constexpr asset MIN_BET(300000, TLOS_SYM);

class [[eosio::contract]] lovebets : public contract {
   public:
      using contract::contract;

      [[eosio::action]]
      void initbet( 
         name minister,
         vector<name> bettors,
         vector<name> witnesses,
         asset loss,
         vector<asset> bettor_quantity
      );
      //using initbet_action = action_wrapper<"wbets"_n, &lovebets::initbet>;


      // Bet in progress ( not already validated on the blockchain)
      struct [[eosio::table]] wannabe_bets_s
      {
         // Table's id
         uint64_t id;
         // People that are going to participate in the bet
         vector<name> bettors;
         // Witnesses of the bet
         vector<name> witnesses;
         // Minister
         name minister;
         // Ammount of money bettors will lose if they divorce
         asset loss;
         // Amount of money each bettor is going to bet
         vector<asset> bettor_quantity;
         uint64_t primary_key() const {return id;}
      };

      typedef eosio::multi_index<"wbets"_n, wannabe_bets_s> wannabe_bets_t;

      [[eosio::on_notify("eosio.token::transfer")]]
      void bet_handler(name from, name to, asset quantity, string memo);
};
