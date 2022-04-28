#include "lovebets.hpp"

// Initialize bet
void lovebets::initbet(
   name minister,
   vector<name> bettors,
   vector<name> witnesses,
   asset loss,
   vector<asset> bettor_quantity,
   name ram_payer) {

   // Authenticates the minister
   require_auth(minister);

   // Add information to the wannabe_bets table
   wannabe_bets_t wbets(get_self(), get_self().value);
   wbets.emplace(ram_payer, [&](auto& row) {
      row.id = wbets.available_primary_key();
      row.bettors = bettors;
      row.unpaid = bettors;
      row.witnesses = witnesses;
      row.minister = minister;
      row.loss = loss;
      row.bettor_quantity = bettor_quantity;
      // Print the id
      print(row.id);
   });
}

void lovebets::bet_handler(
   name from,
   name to,
   asset quantity,
   string memo) {

   // If the transacction was from me or is not for me, do nothing.
   if (to != get_self() || from == get_self()) return;

   // If I recieved the transaction but there is not a bet refund the amount
   // if ()

   // Check quantity symbol is TLOS
   check(quantity.symbol == TLOS_SYM, "Symbol has to be TLOS.");
   // Check quantity asset is at least 30.0000 TLOS
   check(quantity.amount <= MIN_BET, "Minimum quantity required is 30.0000 TLOS");

   wannabe_bets_t wbets(get_self(), get_self().value);

   // Convert string id to uint
   uint64_t memo_id = std::stoull(memo);
   auto bet_it = wbets.find(memo_id);
   // Check if the memo is in the database, if it's not print "Bet not found."
   check(bet_it != wbets.end(), "Bet not found.");

   // Iterator
   auto bettor_it = find(bet_it->bettors.begin(), bet_it->bettors.end(), from);

   // Check if the bettor is in the database, if it's not print "Bettor not found."
   check(bettor_it != bet_it->bettors.end(), "Bettor not found.");

   // Find index
   uint8_t index = bettor_it - bet_it->bettors.begin();

   // Delete bettor from wbet["unpaid"]
   auto remaining = 0;
   wbets.modify(bet_it, get_self(), [&](auto& row) {
      row.unpaid.erase(row.unpaid.begin() + index);
      // row.bettor_quantity.erase(row.bettor_quantity.begin() + index);
      remaining = row.unpaid.size();
   });


   if(remaining == 0)  {

      //Add bettors to bet in progress list
      bets_in_progress_t pbets(get_self(), get_self().value);
      pbets.emplace(from, [&](auto& row) {
         row.id = pbets.available_primary_key();
         row.bettors = bet_it->bettors;
         row.witnesses = bet_it->witnesses;
         row.minister = bet_it->minister;
         row.loss = bet_it->loss;
         row.bettor_quantity = bet_it->bettor_quantity;
         print(row.id);
      });

      //Delete bettors from wbets
      wbets.erase(bet_it);
      print("Bet in progress!");
   }
}

void lovebets::endbet (name bettor, uint64_t bet_id) {

   require_auth(bettor);

   bets_in_progress_t pbets(get_self(), get_self().value);

   auto end_it = pbets.find(bet_id);

   check(end_it != pbets.end(), "Bet not found.");



   //Calculate average of bettors quantity
   for (const asset& quantity : end_it->bettor_quantity) {

   }

   // Send return money to bettors
   int i = 0;
   asset total_loss = asset(0, TLOS_SYM);
   // for (auto& bettor_it = end_it->bettors.begin(); bettor_it != end_it->bettors.end(); bettor_it++) {
   for (const name& bettor : end_it->bettors) {
   // Calculate loss
      asset lost_money = asset_multiply(end_it->loss, end_it->bettor_quantity[i]);
      asset return_quantity = end_it->bettor_quantity[i] - lost_money;

      total_loss += lost_money;

      action(
         permission_level{get_self(), "active"_n},
         "eosio.token"_n,
         "transfer"_n,
         std::make_tuple(get_self(), bettor, return_quantity)
         // string(bettor.to_string() + ("recieved ") + return_quantity.to_string() + (" from finalized bet."))
      ).send();
      print(bettor.to_string() + ("recieved ") + return_quantity.to_string() + (" from finalized bet."));
      i++;
   }

   //Send lost money to system account
   action(
      permission_level{get_self(),"active"_n},
      "eosio.token"_n,
      "transfer"_n,
      std::make_tuple(get_self(), "eosio"_n, total_loss)
      // string("donation from love bet: ") + total_loss.to_string())
  ).send(); 

   print(("donation from love bet: ") + total_loss.to_string());

   // Delete bettors from bets in progress list
   pbets.erase(end_it);
}   




