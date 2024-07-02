#include "Bond.h"
#include "Market.h"

void Bond::generateBondSchedule(std::vector<Date>& cashFlows) const {
  Date paymentDate = startDate;
  while (paymentDate < maturityDate) {
    paymentDate = paymentDate + frequency;
    cashFlows.push_back(paymentDate);
  }
  // Ensure the final maturity date is included
  cashFlows.push_back(maturityDate);
}

double Bond::Payoff(double s) const
{
  double pv = notional * (s - tradePrice);
  return pv;
}

double Bond::Pv(const Market& mkt, int mktDataBool) const {
  // Create a cash flow schedule for the bond
  std::vector<Date> cashFlows;
  generateBondSchedule(cashFlows);


  double pv = 0.0;
  RateCurve rateCurve;

  if (mktDataBool == 1){
    rateCurve = mkt.getCurve_1("USD-SOFR");
  } else{
    rateCurve = mkt.getCurve_2("USD-SOFR");
  }
  // auto rateCurve = mkt.getCurve_1("usd-sofr");

  for (const auto& cashFlowDate : cashFlows) {
    double df = rateCurve.getDf(cashFlowDate);
    double couponPayment = notional * coupon * frequency;
    pv += couponPayment * df;
  }

  // Add the notional payment at maturity
  double dfMaturity = rateCurve.getDf(maturityDate);
  pv += notional * dfMaturity;

  return pv;
}



