cmake_minimum_required(VERSION 3.5)
project(love_bets_contracts VERSION 0.1.0)
set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)
include(EosioWasmToolchain)

add_contract(lovebets lovebets lovebets.cpp)

file(GLOB custom_includes ${CUSTOM_INCLUDES_DIR}/*)
target_include_directories(lovebets PUBLIC ${custom_includes})
