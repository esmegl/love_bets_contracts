#!/usr/bin/env python3
from py_eosio.sugar import collect_stdout, Name

def test_refund(loveb):
    cleos = loveb

    # a = cleos.new_account()
    # b = cleos.new_account()
    # m = cleos.new_account()
    # bet_name = 'qrmsotvpm1sd'
    # bet_name_n = Name(bet_name)

    # bettor_quantity_a = '46.0000 TLOS'
    # bettor_quantity_b = '78.0000 TLOS'

    # ec, _ = cleos.give_token(a, '1000.0000 TLOS')
    # assert ec == 0
    # ec, _ = cleos.give_token(b, '1000.0000 TLOS')
    # assert ec == 0

    # # Init bet
    # owner_perms = [f'{name}@active' for name in (m, b)]
    # owner_perms.sort()

    # proposal = cleos.multi_sig_propose(
    #         m,
    #         owner_perms,
    #         owner_perms,
    #         'lovebets',
    #         'initbet',
    #         {
    #             'bet_name': bet_name,
    #             'minister': m,
    #             'bettors': (a, b),
    #             'witnesses': (),
    #             'loss': '0.5000 LOSS',
    #             'bettor_quantity': (bettor_quantity_a, bettor_quantity_b)
    #         }
    #     )

    # for name in (m, b):
    #     ec, _ = cleos.multi_sig_approve(
    #         m,
    #         proposal,
    #         [f'{name}@active'],
    #         name
    #     )
    
    #     assert ec == 0        

    # ec, out = cleos.multi_sig_exec(
    #         m,
    #         proposal,
    #         f'{m}@active'
    #     )
    # assert ec == 0 

    breakpoint()