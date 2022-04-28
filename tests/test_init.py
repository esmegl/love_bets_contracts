#!/usr/bin/env python3

from py_eosio.sugar import collect_stdout


def test_initbet(loveb):
    cleos = loveb

    a = cleos.new_account()
    b = cleos.new_account()
    m = cleos.new_account()

    ec, _ = cleos.give_token(a, '1000.0000 TLOS')
    assert ec == 0
    ec, _ = cleos.give_token(b, '1000.0000 TLOS')
    assert ec == 0

    bettors = [ a, b ]
    witnesses = []  
    ministers = []
    loss = '0.9000 LOVE'
    btq = ['1.0000 TLOS', '1.0000 TLOS']

    ec, out = cleos.push_action(
        'lovebets',
        'initbet',
        [m, bettors, witnesses, loss, btq],
        f'{m}@active'
    )
    assert ec == 0

    memo_id = int(collect_stdout(out))

    ec, out = cleos.transfer_token(
        a, 'lovebets', '69.0000 TLOS', memo_id)

    assert ec == 0

    rows = [
        row
        for row in cleos.get_table(
            'lovebets', 'lovebets', 'wbets')
        if row['id'] == memo_id
    ]

    assert len(rows) == 1
    assert a not in rows[0]['bettors']

    ec, out = cleos.transfer_token(
        b, 'lovebets', '69.0000 TLOS', memo_id)

    rows = [
        row
        for row in cleos.get_table(
            'lovebets', 'lovebets', 'wbets')
        if row['id'] == memo_id
    ]

    assert ec ==0
    assert b not in rows[0]['bettors']

    