// #pragma once
// #include <iostream>
// #include <string>
// #include <map>
// #include <unordered_map>
// #include <future>
//
// #include "Trade.h"
// #include "Market.h"
// #include "Pricer.h"
//
// using namespace std;
//
// struct MarketShock {
//     string market_id;
//     pair<Date, double> shock; //tenor and value
// };
//
// class CurveDecorator : public Market {
// public:
// 	CurveDecorator(const Market& mkt, const MarketShock& curveShock) : thisMarketUp(mkt), thisMarketDown(mkt)
//     {
//         cout<< "curve decorator is created"<<endl;
//         auto curve_up = thisMarketUp.getCurve_2(curveShock.market_id);
//         curve_up.shock(curveShock.shock.first, curveShock.shock.second);
// 	    thisMarketUp.addCurve_2(curveShock.market_id, curve_up);
//         cout<< "curve tenor " << curveShock.shock.first << "is shocked" << curveShock.shock.second<<endl;
//
//         auto curve_dn = thisMarketDown.getCurve_2(curveShock.market_id);
//         curve_dn.shock(curveShock.shock.first, -1*curveShock.shock.second);
// 	    thisMarketDown.addCurve_2(curveShock.market_id, curve_dn);
//         cout<< "curve tenor " << curveShock.shock.first << "is shocked" << curveShock.shock.second<<endl;
//
//     }
//     inline const Market& getMarketUp() const { return thisMarketUp;}
//     inline const Market& getMarketDown() const { return thisMarketDown;}
//
// private:
// 	Market thisMarketUp;
//     Market thisMarketDown;
// };
//
// class VolDecorator : public Market {
// public:
// 	VolDecorator(const Market& mkt, const MarketShock& volShock) : thisMarketUp(mkt), thisMarketDown(mkt), shockVal(volShock.shock.second)
//     {
// 	    cout<< "vol decorator is created"<<endl;
// 	    auto curveUp = thisMarketUp.getVolCurve_2(volShock.market_id);
// 	    curveUp.shock(volShock.shock.first, shockVal);
// 	    thisMarketUp.addVolCurve_2(volShock.market_id, curveUp);
// 	    cout<< "vol tenor " << volShock.shock.first << " is shocked " << shockVal<<endl;
// 	    cout<< "curve tenor " << volShock.shock.first << " is shocked " << thisMarketUp.getVolCurve_2(volShock.market_id).getVol(volShock.shock.first)<<endl;
// 	    cout<< "curve tenor " << volShock.shock.first << " is shocked " << curveUp.getVol(volShock.shock.first)<<endl;
//
// 	    auto curveDown = thisMarketDown.getVolCurve_2(volShock.market_id);
// 	    curveDown.shock(volShock.shock.first, -1*shockVal);
// 	    thisMarketDown.addVolCurve_2(volShock.market_id, curveDown);
// 	    cout<< "vol tenor " << volShock.shock.first << " is shocked " << shockVal<<endl;
//     }
//
//     inline const Market& getMarketUp() const { return thisMarketUp;}
//     inline const Market& getMarketDown() const { return thisMarketDown;}
//     inline const double& getShockVal() const { return shockVal; };
//
// private:
//     Market thisMarketUp;
//     Market thisMarketDown;
//     double shockVal;
// };
//
// class RiskEngine
// {
//     RiskEngine(const Market& market, double curve_shock, double vol_shock) {
//
//         cout << " risk engine is created .. " << endl;
//
//         auto curves = market.getCurve_2("USD-SOFR");
//         auto vols = market.getVolCurve_2("ATM-Vol");
//
//         for (auto& curve : curves)
//         {
//             for (auto& tenor : curve.getTenors()) {
//                 MarketShock curveShock = {curve.first, make_pair(tenor, curve_shock)};
//                 curveShocks.insert(std::make_pair(string(tenor), CurveDecorator(market, curveShock)));
//             }
//         }
//
//         for (auto& vol : vols)
//         {
//             for (auto& tenor : vol.getTenors()) {
//                 MarketShock volShock = {vol.first, make_pair(tenor, vol_shock)};
//                 volShocks.insert(std::make_pair(string(tenor), VolDecorator(market, volShock)));
//             }
//         }
//     };
//
//     void computeRisk(string riskType, std::unique_ptr<Trade> trade, std::unique_ptr<Pricer> pricer, bool singleThread);
//
//     // map<string, double> getResult() const {
//     //     cout << " risk result: " << endl;
//     //     for (auto& r : result) {
//     //         cout << r.first << ": " << r.second << endl;
//     //     }
//     //     return result;
//     // };
//
//
// private:
//
//     unordered_map<string, CurveDecorator> curveShocks; //tenor, shock
//     unordered_map<string, VolDecorator> volShocks;
//
//     map<string, double> result;
//
// };
//
