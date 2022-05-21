To build container (from 'REPO_ROOT/docker'):

 - sudo docker build -t eosio-cdt:1.7.0 -f Dockerfile.cdt-1.7.0 .

To build contract or recompile (from 'REPO_ROOT/contracts/'):

 - sudo docker run -it --rm --mount type=bind,source="$(pwd)"/lovebets,target=/root/target eosio-cdt:1.7.0 bash -c 'cd build && cmake .. && make -j6'


 To run tests(from repo root):
 - sudo pytest tests/[testname]


 To activate environment(from root file):
 -source env/bin/activate


 To deactivate environment(from root file):
 - deactivate