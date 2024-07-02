#include "Market.h"

class RiskEngine {
    RiskEngine(const Market &market, double shock) {
        //add implementation
    };

    void computeRisk(string riskType, std::shared_ptr<Trade> trade, bool singleThread) {
        result.clear();
        if (singleThread) {
            if (riskType == "vega") {
                for (auto &kv: volSchoks) {
                    // to be added
                }
            }
        } else {
            auto pv_task = [](shared_ptr<Trade> trade, string id, const Market &mkt_up, const Market &mkt_down) {
                double pv_up = trade->Pv(mkt_up);
                double pv_down = trade->Pv(mkt_down);
                double dv01 = (pv_up - pv_down) / 2.0;
                return std::make_pair(id, dv01);
            };
            vector<std::future<std::pair<string, double>>> _futures;
            // calling the above function check_even asynchronously and storing the result in future object
            for (auto &shock: curveShocks) {
                string market_id = shock.first;
                auto mkt_u = shock.second.getMarketUp();
                auto mkt_d = shock.second.getMarketDown();
                _futures.push_back(std::async(std::launch::async, pv_task, trade, market_id, mkt_u, mkt_u));
            }
            for (auto &&fut: _futures) {
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