#include "Swap.h"
#include "Market.h"
#include <cmath>

void Swap::generateSwapSchedule()
{
  Date paymentDate = startDate;
  while (paymentDate < maturityDate) {
    paymentDate = paymentDate + frequency;
    swapSchedule.push_back(paymentDate);
  }
  // Ensure the final maturity date is included
  swapSchedule.push_back(maturityDate);

}

double Swap::Payoff(double s) const {
  return 0.0; // Use market data set 1 for now
}

//not being used at all anywhere???
double Swap::getAnnuity(const Market& mkt, int mktDataBool) const {
  std::vector<Date> payments = swapSchedule;
  double sumDiscFacts = 0.0;
  RateCurve rateCurve = (mktDataBool == 1) ? mkt.getCurve_1("usd-sofr") : mkt.getCurve_2("usd-sofr");

  for (const Date& day : payments) {
    if (day >= mkt.asOf) {
      double r = rateCurve.getRate(day);
      double t = day - mkt.asOf;
      sumDiscFacts += (exp(-(r * t)) * frequency);
    }
  }

  return notional * sumDiscFacts;
}

double Swap::Pv(const Market& mkt, int mktDataBool) const {
  auto thisMkt = const_cast<Market&>(mkt);
  double fixPv = 0;

  RateCurve rateCurve = (mktDataBool == 1) ? mkt.getCurve_1("USD-SOFR") : mkt.getCurve_2("USD-SOFR");
  double df = rateCurve.getDf(maturityDate);
  double fltPv = (-notional + notional * df);

  for (const auto& dt : swapSchedule) {
    if (dt == startDate)
      continue;
    double tau = dt - startDate;
    df = rateCurve.getDf(dt);
    fixPv += notional * tau * tradeRate * df;
  }

  return fixPv + fltPv;
}




