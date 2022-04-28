#include <vector>
#include <string>

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
using namespace eosio;

using std::vector;
using std::string;
using std::find;

#include "utils.hpp"

class [[eosio::contract]] lovebets : public contract {
   public:
      using contract::contract;

      static constexpr symbol TLOS_SYM = symbol("TLOS", 4);
      static constexpr uint32_t MIN_BET = 300000;

      [[eosio::action]]
      void initbet( 
         name minister,
         vector<name> bettors,
         vector<name> witnesses,
         asset loss,
         vector<asset> bettor_quantity,
         name ram_payer
      );
      //using initbet_action = action_wrapper<"wbets"_n, &lovebets::initbet>;

      [[eosio::action]]
      void endbet(name bettor, uint64_t bet_id);

      [[eosio::on_notify("eosio.token::transfer")]]
      void bet_handler(name from, name to, asset quantity, string memo);


      // Bet not already validated on the blockchain
      struct [[eosio::table]] wannabe_bets_s
      {
         // Table's id
         uint64_t id;
         // People that are going to participate in the bet
         vector<name> bettors;
         // Unpaid bettors
         vector<name> unpaid;
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


      //Bet in progress already validated on the blockchain
      struct [[eosio::table]] bets_in_progress_s
      {
         // Table's id
         uint64_t id;
         // Married bettors
         vector<name> bettors;
         // Witnesses of the bet
         vector<name> witnesses;
         // Minister
         name minister;
         // Ammount of money bettors will lose if they divorce
         asset loss;
         // Amount of money each bettor bet
         vector<asset> bettor_quantity;
         uint64_t primary_key() const {return id;}
      };   

      typedef eosio::multi_index<"wbets"_n, wannabe_bets_s> wannabe_bets_t;
      typedef eosio::multi_index<"pbets"_n, bets_in_progress_s> bets_in_progress_t;

};
