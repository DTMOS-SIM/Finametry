#pragma once
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <future>

#include "Trade.h"
#include "Market.h"

using namespace std;

struct MarketShock {
    string market_id;
    pair<Date, double> shock; //tenor and value
};

class CurveDecorator : public Market {
public:
	CurveDecorator(const Market& mkt, const MarketShock& curveShock) : thisMarketUp(mkt), thisMarketDown(mkt)
    {
        cout<< "curve decorator is created"<<endl;
        auto curve = thisMarketUp.getCurve(curveShock.market_id);
        curve.shock(curveShock.shock.first, curveShock.shock.second);
        cout<< "curve tenor " << curveShock.shock.first << "is shocked" << curveShock.shock.second<<endl;

        auto curve = thisMarketDown.getCurve(curveShock.market_id);
        curve.shock(curveShock.shock.first, -1*curveShock.shock.second);
        cout<< "curve tenor " << curveShock.shock.first << "is shocked" << curveShock.shock.second<<endl;

    }
    inline const Market& getMarketUp() const { return thisMarketUp;}
    inline const Market& getMarketDown() const { return thisMarketDown;}

private:
	Market thisMarketUp;
    Market thisMarketDown;
};

class VolDecorator : public Market {
public:
	VolDecorator(const Market& mkt, const MarketShock& volShock) : thisMarket(mkt) 
    {
        cout<< "vol decorator is created"<<endl;
        auto curve = thisMarket.getVolCurve(volShock.market_id);
        curve.shock(volShock.shock.first, volShock.shock.second);
        cout<< "vol tenor " << volShock.shock.first << "is shocked" << volShock.shock.second<<endl;
    }	

    inline const Market& getMarket() const { return thisMarket;}

private:
	Market thisMarket;
};

class RiskEngine 
{
    RiskEngine(const Market& market, double shock) {
        //add implementation

    };
    void computeRisk(string riskType, std::shared_ptr<Trade> trade, bool singleThread) 
    {
        result.clear();
        if (singleThread) {
            if(riskType == "dv01"){
                for (auto& kv: curveShocks) {
                        string market_id = kv.first;
                        auto mkt_u = kv.second.getMarketUp();
                        auto mkt_d = kv.second.getMarketDown();
                        
                        double pv_up = trade->Pv(mkt_u);
                        double pv_down = trade->Pv(mkt_d);
                        double dv01 = (pv_up - pv_down)/2.0;
                        result.emplace(market_id, dv01);
                }
            }

            if(riskType == "vega"){
                for (auto& kv: volSchoks) {
                    // to be added
                }

            }
        }
        else {
            auto pv_task = [](shared_ptr<Trade> trade, string id, const Market& mkt_up, const Market& mkt_down) {
                double pv_up = trade->Pv(mkt_up);
                double pv_down = trade->Pv(mkt_down);
                double dv01 = (pv_up - pv_down)/2.0;
                return std::make_pair(id, dv01);
            };

            vector<std::future<std::pair<string, double>>> _futures;
            // calling the above function check_even asynchronously and storing the result in future object
            for (auto& shock: curveShocks) {
                string market_id = shock.first;
                auto mkt_u = shock.second.getMarketUp();
                auto mkt_d = shock.second.getMarketDown();
                _futures.push_back(std::async(std::launch::async, pv_task, trade, market_id, mkt_u, mkt_u)); 
            }
            
            for (auto && fut: _futures) {
                auto rs = fut.get();
                result.emplace(rs);
            }     

        }
    }

    map<string, double> getResult();


private:

    unordered_map<string, CurveDecorator> curveShocks; //tenor, shock
    unordered_map<string, VolDecorator> volSchoks;
    
    map<string, double> result;
    
};

