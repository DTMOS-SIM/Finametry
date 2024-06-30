cmake_minimum_required(VERSION 3.28)
project(final_proj)

set(CMAKE_CXX_STANDARD 14)

add_executable(final_proj
        AmericanTrade.h
        Bond.cpp
        Bond.h
        Date.cpp
        Date.h
        EuropeanTrade.h
        Factory.h
        helper.h
        main.cpp
        Market.cpp
        Market.h
        Pricer.cpp
        Pricer.h
        Swap.cpp
        Swap.h
        Trade.h
        TreeProduct.h
        Types.h
        Payoff.cpp)
