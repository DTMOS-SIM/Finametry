#include "Market.h"

#include <memory>
#include <sstream>
#include <iostream>
#include <limits>
#include <cmath>
#include <algorithm>

#include "Factory.h"


using namespace std;

void RateCurve::shock(Date tenor, double value)
{
  //complete
}


void RateCurve::addRate(Date tenor, double rate) {
  //consider to check if tenor already exist
  if (true){
    tenors.push_back(tenor);
    rates.push_back(rate);
  }  
}

double RateCurve::getRate(Date tenor) const {
  if (tenors.empty())
    return 0;  // Handle empty tenors

  // Function to convert Date to an integer for comparison
  auto dateToDays = [](const Date& d) {
    return d.year * 365 + d.month * 30 + d.day;  // Simplified conversion
  };

  int tenorDays = dateToDays(tenor);
  int closestLowerIndex = -1;
  int closestUpperIndex = -1;
  int minLowerDiff = std::numeric_limits<int>::max();
  int minUpperDiff = std::numeric_limits<int>::max();

  // Identify the closest lower and upper tenor indices
  for (size_t i = 0; i < tenors.size(); ++i) {
    int diff = dateToDays(tenors[i]) - tenorDays;
    if (diff <= 0 && std::abs(diff) < minLowerDiff) {
      minLowerDiff = std::abs(diff);
      closestLowerIndex = i;
    }
    else if (diff > 0 && diff < minUpperDiff) {
      minUpperDiff = diff;
      closestUpperIndex = i;
    }
  }

  // Edge case handling: if no upper or lower bound found, return closest available
  if (closestLowerIndex == -1 && closestUpperIndex != -1) {
    return rates[closestUpperIndex];
  }
  else if (closestLowerIndex != -1 && closestUpperIndex == -1) {
    return rates[closestLowerIndex];
  }
  else if (closestLowerIndex == -1 && closestUpperIndex == -1) {
    return 0;  // No volatilities available
  }

  // Perform linear interpolation
  double t0 = dateToDays(tenors[closestLowerIndex]);
  double v0 = rates[closestLowerIndex];
  double t1 = dateToDays(tenors[closestUpperIndex]);
  double v1 = rates[closestUpperIndex];

  double interpolatedVol = v0 + (tenorDays - t0) * (v1 - v0) / (t1 - t0);
  return interpolatedVol;
}

double RateCurve::getDf(Date _date) const
{
  double ccr = getRate(_date);
  double t = _date - _asOf;
  return exp(-ccr * t);
}

void RateCurve::display() const {
  cout << "rate curve:" << name << endl;
  for (size_t i=0; i<tenors.size(); i++) {
    cout << tenors[i] << ":" << rates[i] << endl;
  }
  cout << endl;
}


/*
Next class
*/



void VolCurve::shock(Date tenor, double value)
{
  // complete
}

void VolCurve::addVol(const Date& tenor, const double& vol) {
  auto pos = find(tenors.begin(), tenors.end(), tenor);
  if (pos == tenors.end()) {
    tenors.push_back(tenor);
    vols.push_back(vol);
  }
  else {
    int idx = pos - tenors.begin();
    vols[idx] = vol;
  }
}

double VolCurve::getVol(Date tenor) const {
  if (tenors.empty())
    return 0;  // Handle empty tenors

  // Function to convert Date to an integer for comparison
  auto dateToDays = [](const Date& d) {
    return d.year * 365 + d.month * 30 + d.day;  // Simplified conversion
  };

  int tenorDays = dateToDays(tenor);
  int closestLowerIndex = -1;
  int closestUpperIndex = -1;
  int minLowerDiff = std::numeric_limits<int>::max();
  int minUpperDiff = std::numeric_limits<int>::max();

  // Identify the closest lower and upper tenor indices
  for (size_t i = 0; i < tenors.size(); ++i) {
    int diff = dateToDays(tenors[i]) - tenorDays;
    if (diff <= 0 && std::abs(diff) < minLowerDiff) {
      minLowerDiff = std::abs(diff);
      closestLowerIndex = i;
    }
    else if (diff > 0 && diff < minUpperDiff) {
      minUpperDiff = diff;
      closestUpperIndex = i;
    }
  }

  // Edge case handling: if no upper or lower bound found, return closest available
  if (closestLowerIndex == -1 && closestUpperIndex != -1) {
    return vols[closestUpperIndex];
  }
  else if (closestLowerIndex != -1 && closestUpperIndex == -1) {
    return vols[closestLowerIndex];
  }
  else if (closestLowerIndex == -1 && closestUpperIndex == -1) {
    return 0;  // No volatilities available
  }

  // Perform linear interpolation
  double t0 = dateToDays(tenors[closestLowerIndex]);
  double v0 = vols[closestLowerIndex];
  double t1 = dateToDays(tenors[closestUpperIndex]);
  double v1 = vols[closestUpperIndex];

  double interpolatedVol = v0 + (tenorDays - t0) * (v1 - v0) / (t1 - t0);
  return interpolatedVol;
}

void VolCurve::display() const {
  cout << "vol curve: " << name << endl;
  for (size_t i = 0; i < tenors.size(); i++) {
    cout << tenors[i] << ": " << vols[i] * 100 << "%" << endl;
  }
  cout << endl;
}

void Market::Print() const
{
  cout << "market asof: " << asOf << endl;

  cout << "First interest rate curve" << endl;
  for (auto& curve : curves_1) {
    curve.second.display();
  }
  cout << endl;

  cout << "Second interest rate curve" << endl;
  for (auto& curve : curves_2) {
    curve.second.display();
  }

  cout << "First Vol curve" << endl;
  for (auto& vol : vols_1) {
    vol.second.display();
  }
  cout << endl;

  cout << "Second Vol curve" << endl;

  for (auto& vol : vols_2) {
    vol.second.display();
  }
  cout << endl;
  /*
  add display for bond price and stock price
  */
  // cout << "bond prices:" << endl;
  // for (auto& bondPrice : bondPrices) {
  //   cout << bondPrice.first << ": " << bondPrice.second << endl;
  // }
  // cout << endl;
  cout << "first stock prices:" << endl;
  for (auto& stockPrice : stockPrices_1) {
    cout << stockPrice.first << ": " << stockPrice.second << endl;
  }
  cout << endl;

  cout << "second stock prices:" << endl;
  for (auto& stockPrice : stockPrices_2) {
    cout << stockPrice.first << ": " << stockPrice.second << endl;
  }
  cout << endl;
  cout << "market print complete" << endl;
  cout << endl;
}

void Market::addCurve_1(const std::string& curveData, const RateCurve& curve) {
  curves_1.emplace(curveData, curve);
}

void Market::addCurve_2(const std::string& curveData, const RateCurve& curve) {
  curves_2.emplace(curveData, curve);
}

void Market::addVolCurve_1(const std::string& volData, const VolCurve& curve) {
  vols_1.emplace(volData, curve);
}

void Market::addVolCurve_2(const std::string& volData, const VolCurve& curve) {
  vols_2.emplace(volData, curve);
}

// void Market::addBondPrice(const std::string& bondData, double price) {
//   bondPrices.emplace(bondData, price);
// }

void Market::addStockPrice_1(const std::string& stockData, double price) {
  stockPrices_1.emplace(stockData, price);
}

void Market::addStockPrice_2(const std::string& stockData, double price) {
  stockPrices_2.emplace(stockData, price);
}

std::unique_ptr<Trade> Market::addPortfolio(const std::string& line) {
  std::istringstream iss(line);
  std::string token;
  int id;
  std::string type, underlying, start, end, opt1;
  double notional, freq, strike;

  std::getline(iss, token, ';'); id = std::stoi(token);
  std::getline(iss, type, ';');
  std::getline(iss, underlying, ';');
  std::getline(iss, start, ';');
  std::getline(iss, end, ';');
  std::getline(iss, token, ';'); if (token != "null") {notional = std::stod(token);}
  std::getline(iss, token, ';'); if (token != "null") {freq = std::stod(token);}
  std::getline(iss, token, ';'); if (token != "null") {strike = std::stod(token);}
  std::getline(iss, opt1, ';');

  Date startDate = stringToDate(start);
  Date endDate = stringToDate(end);
  OptionType opt = optionTypeFromString(opt1);

  switch (type[0]) {
      case 'b': // bond
        return BondFactory().createTrade(underlying, startDate, endDate, notional, strike, freq, (opt));
      case 's': // swap
        // cout << "added swap traded" << endl;
        return SwapFactory().createTrade(underlying, startDate, endDate, notional, strike, freq, (opt));
      case 'e': // european option
        return EurOptFactory().createTrade(underlying, startDate, endDate, notional, strike, freq, (opt));
      case 'a': // american option
        return AmericanOptFactory().createTrade(underlying, startDate, endDate, notional, strike, freq, (opt));
      default:
        std::cerr << "Error: Unknown trade type '" << type << "'" << std::endl;
        return AmericanOptFactory().createTrade("", Date(0,0,0), Date(0,0,0), 0, 0, 0.0, OptionType::None);
  }

}

OptionType Market::optionTypeFromString(const std::string& optStr) {
  // Convert optStr to lowercase for case-insensitive comparison
  std::string optLower = optStr;
  std::transform(optLower.begin(), optLower.end(), optLower.begin(), ::tolower);

  if (optLower == "call") {
    return OptionType::Call;
  } else if (optLower == "put") {
    return OptionType::Put;
  } else if (optLower == "binarycall") {
    return OptionType::BinaryCall;
  } else if (optLower == "binaryput") {
    return OptionType::BinaryPut;
  } else {
    return OptionType::None;
  }
}


std::ostream& operator<<(std::ostream& os, const Market& mkt)
{
  os << mkt.asOf << std::endl;
  return os;
}

std::istream& operator>>(std::istream& is, Market& mkt)
{
  is >> mkt.asOf;
  return is;
}
