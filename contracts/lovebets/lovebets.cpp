#include "lovebets.hpp"

void lovebets::bet_handler(
   name from,
   name to,
   asset quantity,
   string memo) {

   // If the transacction was from me or is not for me, do nothing.
   if (to != get_self() || from == get_self()) return;

   // Check quantity symbol is TLOS
   check(quantity.symbol == TLOS_SYM, "Symbol has to be TLOS.");

   // if(quantity < MIN_BET) {

   // }

   wannabe_bets_t wbets(get_self(), get_self().value);

   // Convert string id to uint
   uint64_t memo_id = std::stoull(memo);
   auto bet_it = wbets.find(memo_id);
   // Check if the memo is in the database, if it's not print "Bet not found."
   check(bet_it != wbets.end(), "Bet not found.");

   auto bettor_it = find(bet_it->bettors.begin(), bet_it->bettors.end(), from);
   // Check if the bettor is in the database, if it's not print "Bettor not found."
   check(bettor_it != bet_it->bettors.end(), "Bettor not found.");

   // Find index
   uint8_t index = bettor_it - bet_it->bettors.begin();
   
   // Delete bettor from listss
   wbets.modify(bet_it, get_self(), [&](auto& row) {
      row.bettors.erase(row.bettors.begin() + index);
      row.bettor_quantity.erase(row.bettor_quantity.begin() + index);
   });
      

   // print("Got: ", quantity);
}

// Initialize bet
void lovebets::initbet(
   name minister,
   vector<name> bettors,
   vector<name> witnesses,
   asset loss,
   vector<asset> bettor_quantity) {

   // Authenticates the minister
   require_auth(minister);

   // Table
   wannabe_bets_t wbets(get_self(), get_self().value);
   wbets.emplace(get_self(), [&](auto& row) {
      row.id = wbets.available_primary_key();
      row.bettors = bettors;
      row.witnesses = witnesses;
      row.minister = minister;
      row.loss = loss;
      row.bettor_quantity = bettor_quantity;
      // Print the id
      print(row.id);
   });

}
