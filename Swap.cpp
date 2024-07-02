#include "Swap.h"
#include "Market.h"
#include <cmath>

void Swap::generateSwapSchedule()
{
  if (startDate == maturityDate || frequency <= 0 || frequency > 1)
        throw std::runtime_error("Error: start date is later than end date, or invalid frequency!");
    
    string tenorStr;
    if (frequency == 0.25)
      tenorStr="3M";
    else if (frequency == 0.5) 
      tenorStr="6M";
    else
      tenorStr="1Y";
    
    Date seed = startDate;
    while(seed < maturityDate){
        swapSchedule.push_back(seed);
        seed=dateAddTenor(seed, tenorStr);
    }
    swapSchedule.push_back(maturityDate);
    if (swapSchedule.size()<2)
        throw std::runtime_error("Error: invalid schedule, check input!");

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

  RateCurve rateCurve = (mktDataBool == 1) ? mkt.getCurve_1("usd-sofr") : mkt.getCurve_2("usd-sofr");
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




