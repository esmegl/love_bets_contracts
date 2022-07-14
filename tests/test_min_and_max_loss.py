#!/usr/bin/env python3

from py_eosio.sugar import collect_stdout
import json

def test_min_and_max_loss(loveb):
    cleos = loveb

    a = cleos.new_account()
    b = cleos.new_account()
    m = cleos.new_account()
    bet_name_min = 'nrkutsdtbdee'
    bet_name_max = 'tbyedaunfdwe'

    ec, _ = cleos.give_token(a, '1000.0000 TLOS')
    assert ec == 0
    ec, _ = cleos.give_token(b, '1000.0000 TLOS')
    assert ec == 0

    bettor_quantity = '45.0000 TLOS'

    # Minimum loss
    owner_perms = [f'{name}@active' for name in (m, b)]
    owner_perms.sort()

    proposal = cleos.multi_sig_propose(
            m,
            owner_perms,
            owner_perms,
            'lovebets',
            'initbet',
            {
                'bet_name': bet_name_min,
                'minister': m,
                'bettors': (a, b),
                'witnesses': (),
                'loss': '0.0000 LOSS',
                'bettor_quantity': (bettor_quantity, bettor_quantity)
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
        a, 'lovebets', bettor_quantity, bet_name_min, retry=0)

    assert ec == 1
    assert "Minimum loss required is 0.1000 LOSS." in out


    # Maximum loss
    owner_perms = [f'{name}@active' for name in (m, b)]
    owner_perms.sort()

    proposal = cleos.multi_sig_propose(
            m,
            owner_perms,
            owner_perms,
            'lovebets',
            'initbet',
            {
                'bet_name': bet_name_max,
                'minister': m,
                'bettors': (a, b),
                'witnesses': (),
                'loss': '2.0000 LOSS',
                'bettor_quantity': (bettor_quantity, bettor_quantity)
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
        a, 'lovebets', bettor_quantity, bet_name_max, retry=0)

    assert ec == 1
    assert "Maximum loss allowed is 1 LOSS." in out