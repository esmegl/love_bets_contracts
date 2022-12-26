To build container (from 'REPO_ROOT/docker'):

 - sudo docker build -t eosio-cdt:1.7.0 -f Dockerfile.cdt-1.7.0 .

To build contract or recompile (from 'REPO_ROOT/contracts/'):

 - sudo docker run -it --rm --mount type=bind,source="$(pwd)"/lovebets,target=/root/target eosio-cdt:1.7.0 bash -c 'cd build && cmake .. && make -j6'


 To run tests(from repo root):
 - sudo pytest tests/[testname]


Deploy contract
- cleos --url=https://testnet.telos.net set contract esmeesmeesme /root/contracts/lovebets lovebets.wasm lovebets.abi

Create wallet
- cleos wallet create --to-console

Password: PW5KbmkV6rHiwKKAoH9RKdE9BmosMFaD3RHXFYMXv4RKgb3Nx2kdy


Open shell bash
- sudo docker run -it guilledk/py-eosio:eosio-2.1.0 bash

Talk with testnet
- cleos --url=https://testnet.telos.net 

Get info
- get info


Import private keys
- cleos wallet 


Complete guide for deploying
- https://developers.eos.io/manuals/eos/v2.0/cleos/how-to-guides/index