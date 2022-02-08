cmake_minimum_required(VERSION 3.21)
project(demo_bill_acceptor)

set(CMAKE_CXX_STANDARD 14)

add_executable(demo_bill_acceptor main.cpp state.h tinyfsm.hpp)
