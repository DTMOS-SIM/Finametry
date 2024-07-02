#include <cmath>
#include "Pricer.h"


double Pricer::Price(const Market& mkt, std::unique_ptr<Trade> trade) 
{
  double pv_1 = 0.0;
  double pv_2 = 0.0;
  double PnL = 0.0;
  if (trade->getType() == "TreeProduct") {
    auto treePtr = dynamic_cast<TreeProduct*>(trade.get());
    if (treePtr) { //check if cast is successful
      pv_1 = PriceTree(mkt, *treePtr, 1);
      pv_2 = PriceTree(mkt, *treePtr, 2);
      PnL = pv_2 - pv_1;
    }
  }
  else if (trade->getType() == "BondTrade")
  {
    pv_1 = trade->Pv(mkt,1);
    pv_2 = trade->Pv(mkt,2);
    PnL = pv_2 - pv_1;
  }
  else if (trade->getType() == "SwapTrade")
  {
    pv_1 = trade->Pv(mkt,1);
    pv_2 = trade->Pv(mkt,2);
    PnL = pv_2 - pv_1;
  }
  return PnL;
}

double BinomialTreePricer::PriceTree(const Market& mkt, const TreeProduct& trade, const int& marketDataBool)
{
  // model setup
  double T = trade.GetExpiry() - mkt.asOf;
  double dt = T / nTimeSteps;
  double stockPrice1, stockPrice2, vol1, vol2, rate1, rate2;
  string rateCurve = "USD-SOFR";
  string volCurve = "ATM-Vol";
  /*
  get these data for the deal from market object
  */

  if (marketDataBool == 1) {
      stockPrice1 = mkt.getStockPrice_1(trade.getName());
      rate1 = mkt.getCurve_1(rateCurve).getRate(trade.GetExpiry());
      vol1 = mkt.getVolCurve_1(volCurve).getVol(trade.GetExpiry());
      ModelSetup(stockPrice1,vol1, rate1, dt);

      // initialize
      for (int i = 0; i <= nTimeSteps; i++) {
          states[i] = trade.Payoff( GetSpot(nTimeSteps, i) );
      }

      // price by backward induction
      for (int k = nTimeSteps-1; k >= 0; k--)
          for (int i = 0; i <= k; i++) {
              // calculate continuation value
              double df = exp(-rate1 *dt);
              double continuation = df * (states[i]*GetProbUp() + states[i+1]*GetProbDown());
              // calculate the option value at node(k, i)
              states[i] = trade.ValueAtNode( GetSpot(k, i), dt*k, continuation);
          }

      return states[0];
  }
  else {
      stockPrice2 = mkt.getStockPrice_2(trade.getName());
      rate2 = mkt.getCurve_2(rateCurve).getRate(trade.GetExpiry());
      vol2 = mkt.getVolCurve_2(volCurve).getVol(trade.GetExpiry());
      ModelSetup(stockPrice2,vol2, rate2, dt);

      // initialize
      for (int i = 0; i <= nTimeSteps; i++) {
          states[i] = trade.Payoff( GetSpot(nTimeSteps, i) );
      }

      // price by backward induction
      for (int k = nTimeSteps-1; k >= 0; k--)
          for (int i = 0; i <= k; i++) {
              // calculate continuation value
              double df = exp(-rate2 *dt);
              double continuation = df * (states[i]*GetProbUp() + states[i+1]*GetProbDown());
              // calculate the option value at node(k, i)
              states[i] = trade.ValueAtNode( GetSpot(k, i), dt*k, continuation);
          }

      return states[0];
  }

}

void CRRBinomialTreePricer::ModelSetup(double S0, double sigma, double rate, double dt)
{
  double b = std::exp((2*rate+sigma*sigma)*dt)+1;
  u = (b + std::sqrt(b*b - 4*std::exp(2*rate*dt))) / 2 / std::exp(rate*dt);
  p = (std::exp(rate*dt) -  1/u) / (u - 1/u);
  currentSpot = S0;
}

void JRRNBinomialTreePricer::ModelSetup(double S0, double sigma, double rate, double dt)
{
    u = std::exp( (rate - sigma*sigma/2) * dt + sigma * std::sqrt(dt) );
    d = std::exp( (rate - sigma*sigma/2) * dt - sigma * std::sqrt(dt) );
    p = (std::exp(rate*dt) -  d) / (u - d);
    currentSpot = S0;
}
