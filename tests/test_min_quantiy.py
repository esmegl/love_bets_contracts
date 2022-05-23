#!/usr/bin/env python3

from py_eosio.sugar import collect_stdout, Name
import json

def test_initbet(loveb):
    cleos = loveb

    a = cleos.new_account()
    b = cleos.new_account()
    m = cleos.new_account()
    bet_name = 'asdfghjklouw'

    ec, _ = cleos.give_token(a, '1000.0000 TLOS')
    assert ec == 0
    ec, _ = cleos.give_token(b, '1000.0000 TLOS')
    assert ec == 0

    bettor_quantity_a = '13.0000 TLOS'
    bettor_quantity_b = '40.0000 TLOS'

    # Check min quantity
    owner_perms = [f'{name}@active' for name in (m, b)]
    owner_perms.sort()

    proposal = cleos.multi_sig_propose(
            m,
            owner_perms,
            owner_perms,
            'lovebets',
            'initbet',
            {
                'bet_name': bet_name,
                'minister': m,
                'bettors': (a, b),
                'witnesses': (),
                'loss': '0.9000 LOSS',
                'bettor_quantity': (bettor_quantity_a, bettor_quantity_b)
            }
        )

    for name in (m, b):
        ec, _ = cleos.multi_sig_approve(
            m,
            proposal,
            [f'{name}@active'],
            name
        )
    
        assert ec == 0        

    ec, out = cleos.multi_sig_exec(
            m,
            proposal,
            f'{m}@active'
        )
    assert ec == 0 

    ec, out = cleos.transfer_token(
        a, 'lovebets', bettor_quantity_a, bet_name, retry=0)

    assert ec == 1
    assert 'Minimum quantity required is 30.0000 TLOS.' in out
