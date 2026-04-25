cmake_minimum_required(VERSION 3.10)
project(KVStore)

set(CMAKE_CXX_STANDARD 17)

add_executable(kv_store 
    src/main.cpp
    src/storage/kv_store.cpp
    src/network/tcp_server.cpp
)

target_link_libraries(kv_store ws2_32)