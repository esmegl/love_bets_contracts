#!/usr/bin/env python3

from py_eosio.sugar import collect_stdout, Name
import json


def test_end_bet(loveb):
    cleos = loveb

    a = cleos.new_account()
    b = cleos.new_account()
    m = cleos.new_account()
    bet_name = 'dwidtbadoemd'
    bet_name_n = Name(bet_name)

    ec, _ = cleos.give_token(a, '1000.0000 TLOS')
    assert ec == 0
    ec, _ = cleos.give_token(b, '1000.0000 TLOS')
    assert ec == 0

    bettor_quantity_a = '69.0000 TLOS'
    bettor_quantity_b = '30.0000 TLOS'

    # Multi signature
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
        a, 'lovebets', bettor_quantity_a, bet_name)
    assert ec == 0

    ec, out = cleos.transfer_token(
        b, 'lovebets', bettor_quantity_b, bet_name)
    assert ec == 0
    
    rows = [
            row
            for row in cleos.get_table(
                'lovebets', 'lovebets', 'pbets')
            if row['id'] == str(bet_name_n.value)
    ]

    assert len(rows) == 1
    assert a, b in rows[0]['bettors'] 


    # Call endbet function
    ec, out = cleos.push_action(
        'lovebets',
        'endbet',
        [a, bet_name],
        f'{a}@active'
    )    
    
    assert ec == 0

    # Check the table was deleted from pbets
    rows = [
            row
            for row in cleos.get_table(
                'lovebets', 'lovebets', 'pbets')
            if row['id'] == str(bet_name_n.value)
    ]

    assert len(rows) == 0




