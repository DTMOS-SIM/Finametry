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
    double GetPrice() const override { return tradePrice;}
    string GetNotional() const override {return std::to_string(notional);}
    inline const Date& GetExpiry() const override { return maturityDate; }

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