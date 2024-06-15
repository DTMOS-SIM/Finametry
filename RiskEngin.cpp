#include "RiskEngin.h"
#include "Trade.h"
#include "Pricer.h"
#include "Factory.h"

int main() 
{
    // some sample code to demo
    
    RiskEngine re;
    string risk_id = "usd-sofr:on";
    double shockUp = 0.0001;
    double shockDown = -0.0001;
    auto testShockUp = MarketShock();
    testShockUp.market_id ="usd-sofr";
    testShockUp.shock = make_pair(Date(), shockUp);
    auto testShockDown = MarketShock();
    testShockDown.market_id ="usd-sofr";
    testShockDown.shock = make_pair(Date(), shockDown);

    Market testMarketData;//original market
    auto shockedUpCurveUp = CurveDecorator(testMarketData, testShockUp);
    auto shockedUpCurveDown = CurveDecorator(testMarketData, testShockDown);

    auto sFactory = std::make_unique<SwapFactory>();
    auto swap = sFactory->createTrade("usd-sofr",Date(2024, 1, 1), Date(2034, 1, 1),-1000000, 0.03, 1.0, OptionType::None);

    unordered_map<string, double> thisDealDv01;
    double pv_up, pv_down;
    pv_up = swap->Pv(shockedUpCurveUp.getMarket());
    pv_down = swap->Pv(shockedUpCurveDown.getMarket());
    double dv01 = (pv_up - pv_down)/2.0;
    thisDealDv01.emplace(risk_id, dv01);
}