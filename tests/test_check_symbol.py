#!/usr/bin/env python3

from py_eosio.sugar import collect_stdout, Name
import json

def test_initbet(loveb):
    cleos = loveb

    a = cleos.new_account()
    b = cleos.new_account()
    m = cleos.new_account()
    bet_name_tlos = 'hewvutlposrc'
    bet_name_loss = 'wabgrtunfdsf'

    ec, _ = cleos.give_token(a, '1000.0000 TLOS')
    assert ec == 0
    ec, _ = cleos.give_token(b, '1000.0000 TLOS')
    assert ec == 0

    bettor_quantity_tlos = '76.0000 CRYPTO'
    bettor_quantity_loss = '56.0000 TLOS'

    # Check TLOS symbol
    owner_perms = [f'{name}@active' for name in (m, b)]
    owner_perms.sort()

    proposal = cleos.multi_sig_propose(
            m,
            owner_perms,
            owner_perms,
            'lovebets',
            'initbet',
            {
                'bet_name': bet_name_tlos,
                'minister': m,
                'bettors': (a, b),
                'witnesses': (),
                'loss': '0.9000 LOSS',
                'bettor_quantity': (bettor_quantity_tlos, bettor_quantity_loss)
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
        a, 'lovebets', bettor_quantity_tlos, bet_name_tlos, retry=0)

    assert ec == 1
    assert 'Symbol has to be TLOS.' in out

    cleos.logger.critical(f'Out is: {out}')

    # Check LOSS symbol
    # owner_perms = [f'{name}@active' for name in (m, b)]
    # owner_perms.sort()

    proposal = cleos.multi_sig_propose(
            m,
            owner_perms,
            owner_perms,
            'lovebets',
            'initbet',
            {
                'bet_name': bet_name_loss,
                'minister': m,
                'bettors': (a, b),
                'witnesses': (),
                'loss': '0.7000 LOVE',
                'bettor_quantity': (bettor_quantity_loss, bettor_quantity_loss)
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
        a, 'lovebets', bettor_quantity_loss, bet_name_loss, retry=0)

    assert ec == 1
    assert 'Symbol has to be LOSS.' in out