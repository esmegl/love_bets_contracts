#!/usr/bin/env python3

import pytest

from py_eosio.fixtures import single_node_chain as cleos


@pytest.fixture(scope='session')
def loveb(cleos):
    cleos.create_account_staked(
        'eosio', 'lovebets', ram=480278)
    
    cleos.deploy_contract_from_host(
        'lovebets',
        'contracts/lovebets/build',
        create_account=False,
        verify_hash=False
    )
    yield cleos