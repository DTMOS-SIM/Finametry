#pragma once
#include <iostream>
#include "Swap.h"
#include "Bond.h"
#include "EuropeanTrade.h"
#include "AmericanTrade.h"

// Abstract creator class
class TradeFactory {
public:
    virtual std::unique_ptr<Trade> createTrade(std::string underlying, Date start, Date end, double notional, double strike, double freq, OptionType opt) = 0;
    virtual ~TradeFactory() = default; // Virtual destructor for polymorphism
};

// Concrete creator class - SwapFactory
class SwapFactory : public TradeFactory {
public:
    std::unique_ptr<Trade> createTrade(std::string underlying, Date start, Date end, double notional, double strike, double freq, OptionType opt) override {
        return std::make_unique<Swap>(underlying, start, end, notional, strike, freq);//implement this
    }
};

// Concrete creator class - BondFactory
class BondFactory : public TradeFactory {
public:
    std::unique_ptr<Trade> createTrade(std::string underlying, Date start, Date end, double notional, double strike, double freq, OptionType opt) override {
        return std::make_unique<Bond>(underlying, start, end, notional, strike, freq);// implement this
    }
};

// Concrete creator class - EuropeanFactory
class EurOptFactory : public TradeFactory {
public:
    std::unique_ptr<Trade> createTrade(string underlying, Date start, Date end, double notional, double strike, double freq, OptionType opt) override {
        return std::make_unique<EuropeanOption>(opt, strike, end, underlying); //implement this
    }
};

// Concrete creator class - AmericanOptFactory
class AmericanOptFactory : public TradeFactory {
public:
    std::unique_ptr<Trade> createTrade(std::string underlying, Date start, Date end, double notional, double strike, double freq, OptionType opt) override {
        return std::make_unique<AmericanOption>(opt, strike, end, underlying); //implement this
    }
};