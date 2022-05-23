#include "lovebets.hpp"

// Initialize bet
void lovebets::initbet(
   name bet_name,
   name minister,
   vector<name> bettors,
   vector<name> witnesses,
   asset loss,
   vector<asset> bettor_quantity) {

   require_auth(minister);

   wannabe_bets_t wbets(get_self(), get_self().value);

   //Check if bet name is not taken already
   check(wbets.find(bet_name.value) == wbets.end(), 
      "Bet name already taken, please choose another name. \n");

   // The ram payer is the last bettor to enter the bet
   name ram_payer = bettors[bettors.size() - 1];

   // Add information to the wannabe_bets table
   wbets.emplace(ram_payer, [&](auto& row) {
      row.id = bet_name.value;
      row.bettors = bettors;
      row.unpaid = bettors;
      row.witnesses = witnesses;
      row.minister = minister;
      row.loss = loss;
      row.bettor_quantity = bettor_quantity;
      row.ram_payer = ram_payer;
      // Print the id
      print(row.id);
   });


   // Initialize table for bets in progress
   bets_in_progress_t pbets(get_self(), get_self().value);
   pbets.emplace(ram_payer, [&](auto& row) {
      row.id = bet_name.value;
      row.bettors = vector<name> (bettors.size());
      row.witnesses = vector<name> (witnesses.size());
      row.minister = ""_n;
      row.loss = asset(0, LOSS_SYM);
      row.bettor_quantity = vector<asset> (bettor_quantity.size());
   });
}

// Cancel bet initialization
void lovebets::cancelbet(name bettor, string bet_name) {

   require_auth(bettor);

   wannabe_bets_t wbets(get_self(), get_self().value);

   name n = name(bet_name);
   auto cancel_it = wbets.find(n.value);
   check(cancel_it != wbets.end(), "Bet not found.\n");

   wbets.erase(cancel_it);
   print("Bet cancelled. \n");
}

void lovebets::bet_handler(
   name from,
   name to,
   asset quantity,
   string bet_name) {

   // If the transacction was from lovebets or is not for lovebets, do nothing.
   if (to != get_self() || from == get_self()) return;

   //TODO:
   // If I recieved the transaction but there is not a bet refund the amount
   // if ()

   check(quantity.symbol == TLOS_SYM, "Symbol has to be TLOS.\n");
   check(quantity.amount >= MIN_BET, "Minimum quantity required is 30.0000 TLOS.\n");
   wannabe_bets_t wbets(get_self(), get_self().value);

   // Convert string id to uint
   name n = name(bet_name);
   auto bet_it = wbets.find(n.value);

   auto ram_payer = bet_it->ram_payer;

   asset loss = bet_it->loss;
   check(loss.symbol == LOSS_SYM, "Symbol has to be LOSS.\n");
   check(loss.amount >= MIN_LOSS, "Minimum loss required is 0.1000 LOSS.\n");
   check(loss.amount <= MAX_LOSS, "Maximum loss allowed is 1 LOSS.\n");

   // Check if the memo is in the database, if it's not print "Bet not found."
   check(bet_it != wbets.end(), "Bet not found.\n");

   // Betor iterator
   auto bettor_it = find(bet_it->bettors.begin(), bet_it->bettors.end(), from);

   // Check if the bettor is in the database, if it's not print "Bettor not found."
   check(bettor_it != bet_it->bettors.end(), "Bettor not found.\n");

   int i = bettor_it - bet_it->bettors.begin();
   check(quantity == bet_it->bettor_quantity[i], 
      "The amount of TLOS is incorrect, please transfer the amount stated in the initialization of the bet. \n");

   // Unpaid iterator
   auto unpaid_it = find(bet_it->unpaid.begin(), bet_it->unpaid.end(), from);

   // Delete bettor from wbet["unpaid"]
   auto remaining = 0;
   wbets.modify(bet_it, get_self(), [&](auto& row) {
      row.unpaid.erase(unpaid_it);

      remaining = row.unpaid.size();
   });


   // When all bettors paid, delete them from wbets and add them to pbets
   if(remaining == 0)  {

      //Add bettors to bet in progress list
      bets_in_progress_t pbets(get_self(), get_self().value);
      auto pbet_it = pbets.find(n.value);
      pbets.modify(pbet_it, ram_payer, [&](auto& row) {
         row.bettors = bet_it->bettors;
         row.witnesses = bet_it->witnesses;
         row.minister = bet_it->minister;
         row.loss = bet_it->loss;
         row.bettor_quantity = bet_it->bettor_quantity;
         print(row.id);
         print("Bet in progress, table updated succesfully! \n");
      });

      //Delete bettors from wbets
      wbets.erase(bet_it);
      print("Deleted data in wannabe bets table. \n");
   }
}

// End initialized bet
void lovebets::endbet (name bettor, string bet_name) {

   require_auth(bettor);

   bets_in_progress_t pbets(get_self(), get_self().value);

   name n = name(bet_name);
   auto end_it = pbets.find(n.value);

   check(end_it != pbets.end(), "Bet not found.\n");

   // Send return money to bettors
   int i = 0;
   asset total_loss = asset(0, TLOS_SYM);
   for (const name& bettor : end_it->bettors) {
      // Calculate loss
      asset lost_money = asset_multiply(end_it->loss, end_it->bettor_quantity[i]);
      asset return_quantity = end_it->bettor_quantity[i] - lost_money;

      total_loss += lost_money;

      action(
         permission_level{get_self(), "active"_n},
         "eosio.token"_n,
         "transfer"_n,
         std::make_tuple(get_self(), bettor, return_quantity, bet_name)
      ).send();
      print(bettor.to_string() + ("recieved ") + return_quantity.to_string() + (" from finalized bet.") + ("\n"));
      i++;
   }

   //Send lost money to system account
   action(
      permission_level{get_self(),"active"_n},
      "eosio.token"_n,
      "transfer"_n,
      std::make_tuple(get_self(), "eosio"_n, total_loss, bet_name)
  ).send(); 

   print(("Donation from love bet: ") + total_loss.to_string() + ("\n"));

   // Delete bettors from bets in progress list
   pbets.erase(end_it);
   
}   




