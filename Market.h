#ifndef MARKET_H
#define MARKET_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "Date.h"

using namespace std;

class RateCurve {
public:
  RateCurve(){};
  RateCurve(const string& _name): name(_name) {} ;
  void shock(Date tenor, double value); //implement this
  void addRate(Date tenor, double rate);
  double getRate(Date tenor) const; //implement this function using linear interpolation
  double getDf(Date _date) const; // using df = exp(-rt), and r is getRate function
  void display() const;
  
  std::string name;
  Date _asOf;//same as market data date

private:
  vector<Date> tenors;
  vector<double> rates; //zero coupon rate or continous compounding rate
};

class VolCurve { // atm vol curve without smile
public:
  VolCurve(){}
  VolCurve(const string& _name): name(_name) {} ;
  void shock(Date tenor, double value); //implement this
  void addVol(const Date& tenor,const double& rate); // implement this
  double getVol(Date tenor) const; //implement this function using linear interpolation
  void display() const; //implement this

  
  string name;
  Date _asOf;

private:
  vector<Date> tenors;
  vector<double> vols;
};

class Market
{
public:
  Date asOf;
  Market(){
    cout<< "default constructor is called" <<endl;
  };
  Market(const Date& now): asOf(now) {};
  Market(const Market& mkt){
    this->asOf = mkt.asOf;
    //etc


  } //implement this

  void Print() const;
  void addCurve_1(const std::string& curveName, const RateCurve& curve);//implement this
  void addCurve_2(const std::string& curveName, const RateCurve& curve);//implement this
  void addVolCurve_1(const std::string& curveName, const VolCurve& curve);//implement this
  void addVolCurve_2(const std::string& curveName, const VolCurve& curve);//implement this
  // void addBondPrice(const std::string& bondName, double price);//implement this
  void addStockPrice_1(const std::string& stockName, double price);//implement this
  void addStockPrice_2(const std::string& stockName, double price);//implement this
  double getMarketPrice() const;
  
    inline RateCurve getCurve(const string& name) const { return curves_1.at(name); };
    inline VolCurve getVolCurve(const string& name) const { return vols_1.at(name); };
    inline double getStockPrice(const string& name) const { return stockPrices_1.at(name); };
    // inline double getBondPrice(const string& name) const { return bondPrices_1.at(name); };

private:

  unordered_map<string, RateCurve> curves_1;
  unordered_map<string, RateCurve> curves_2;
  unordered_map<string, VolCurve> vols_1;
  unordered_map<string, VolCurve> vols_2;
  // unordered_map<string, double> bondPrices;
  unordered_map<string, double> stockPrices_1;
  unordered_map<string, double> stockPrices_2;
};

std::ostream& operator<<(std::ostream& os, const Market& obj);
std::istream& operator>>(std::istream& is, Market& obj);

#endif
