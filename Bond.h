#pragma once
#include "Trade.h"

class Bond : public Trade {
public:
    Bond(std::string name, Date start, Date end, double _notional, double rate, double freq): Trade("BondTrade", start) {
        underlying = name;
        notional = _notional;
        frequency = freq;
        coupon = rate;
        startDate = start;
        maturityDate = end;
    }
    double Payoff(double s) const override; // implement this
    double Pv(const Market& mkt, int mktDataBool) const override; // implement this
    void generateBondSchedule(vector<Date>& cashFlows) const; //implement this
    inline virtual string getName() const override { return underlying; }

private:
    std::string underlying;
    double notional;
    double tradePrice;
    double coupon;
    double frequency;

    Date startDate;
    Date maturityDate;
    vector<Date> cashFlows;

};