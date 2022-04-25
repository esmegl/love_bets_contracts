#!/usr/bin/env python3


from pytest_eosio.telos import init_telos_token
from pytest_eosio.sugar import collect_stdout


def test_initbet(eosio_testnet):
    init_telos_token(eosio_testnet)

    eosio_testnet.create_account('eosio', 'a')
    eosio_testnet.create_account('eosio', 'b')
    eosio_testnet.create_account('eosio', 'm')

    ec, _ = eosio_testnet.give_token('a', '1000.0000 TLOS')
    assert ec == 0
    ec, _ = eosio_testnet.give_token('b', '1000.0000 TLOS')
    assert ec == 0

    bettors = [ 'a', 'b' ]
    witnesses = []  
    ministers = []
    loss = '0.9000 LOVE'
    btq = ['1.0000 TLOS', '1.0000 TLOS']

    ec, out = eosio_testnet.push_action(
        'lovebets',
        'initbet',
        ['m', bettors, witnesses, loss, btq],
        'm@active'
    )
    assert ec == 0

    memo_id = collect_stdout(out)

    ec, out = eosio_testnet.transfer_token(
        'a', 'lovebets', '69.0000 TLOS', memo_id)


    rows = eosio_testnet.get_table(
        'lovebets', 'lovebets', 'wbets')

    breakpoint()