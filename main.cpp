#include <fstream>
#include <ctime>
#include <chrono>
#include <regex>

#include "Market.h"
#include "Pricer.h"
#include "EuropeanTrade.h"
#include "Bond.h"
#include "Swap.h"
#include "AmericanTrade.h"
#include "Factory.h"
// blah



using namespace std;

void readFromFile(const string& fileName, string& title, vector< pair<string, double> >& values) {
    string lineText, tmp_key, key, delimiter = ":";
    double value;
    size_t pos = 0;
    regex tr("ON|(\\d+[MY])");
    smatch m;

    ifstream MyReadFile(fileName);
    while (getline(MyReadFile, lineText)) {
        if (lineText.find(delimiter) == string::npos) {
            title.append(lineText);
        }
        else {
            // Check if first line consists of curve name
            while ((pos = lineText.find(":")) != string::npos) {
                tmp_key = lineText.substr(0, pos);
                // Check if keys are of different tenors
                // If they are, convert them into year fractions
                if (regex_match(tmp_key, m, tr)) {
                    switch (tmp_key.back()) {
                        case 'N':
                            key = to_string(1.0 / 365.0);
                            break;
                        case 'M':
                            key = to_string(stod(tmp_key.substr(0, tmp_key.size() - 1)) / 12.0);
                            break;
                        case 'Y':
                            key = to_string(stod(tmp_key.substr(0, tmp_key.size() - 1)));
                            break;
                    }
                }
                else {
                    key = tmp_key;
                }
                lineText.erase(0, pos + delimiter.length() + 1);
                // Check if values are in percentages
                if (lineText.back() == '%') {
                    value = stod(lineText.substr(0, lineText.size() - 1)) / 100;
                }
                else {
                    value = stod(lineText);
                }
                values.push_back(make_pair(key, value));
            }
        }
    }
    MyReadFile.close();
}

int main()
{
  //task 1, create an market data object, and update the market data from from txt file
  // load 2 days market data
  std::time_t t = std::time(nullptr);
  auto now_ = std::localtime(&t);
  Date valueDate;
  valueDate.year = now_->tm_year + 1900;
  valueDate.month = now_->tm_mon + 1;
  valueDate.day = now_->tm_mday;

  Market mkt = Market(valueDate);

    std::string str = "2024-06-01";

    Date testdate = stringToDate(str);
    cout << testdate << endl;

    string basePath = "../market_data/";

    // Read and add stock prices
    string stockname_1;
    std::string stockPricePath_1 = basePath + "stock_price_20240601.txt";
    vector< pair<string, double> >stockData_1;

    readFromFile(stockPricePath_1,stockname_1, stockData_1);
    for (auto& stockPrice : stockData_1) {
        mkt.addStockPrice_1(stockPrice.first, stockPrice.second);
    }

    // Read and add stock prices
    string stockname_2;
    std::string stockPricePath_2 = basePath + "stock_price_20240602.txt";
    vector< pair<string, double> >stockData_2;

    readFromFile(stockPricePath_2,stockname_2, stockData_2);
    for (auto& stockPrice : stockData_2) {
        mkt.addStockPrice_2(stockPrice.first, stockPrice.second);
    }

    // Read and add volatility data
    string volname_1;
    std::string volPath_1 = basePath + "vol_20240601.txt";
    vector< pair<string, double> >volData_1;

    readFromFile(volPath_1,volname_1, volData_1);
    if (volname_1.empty()) {
        volname_1 = "ATM-Vol";
    }
    VolCurve volCurve_1(volname_1);
    for (auto& tenorVol : volData_1) {
        volCurve_1.addVol(valueDate + stod(tenorVol.first), tenorVol.second);
    }
    mkt.addVolCurve_1(volname_1, volCurve_1);

    // Read and add volatility data
    string volname_2;
    std::string volPath_2 = basePath + "vol_20240602.txt";
    vector< pair<string, double> >volData_2;

    readFromFile(volPath_2,volname_2, volData_2);
    if (volname_2.empty()) {
        volname_2 = "ATM-Vol";
    }
    VolCurve volCurve_2(volname_2);
    for (auto& tenorVol : volData_2) {
        volCurve_2.addVol(valueDate + stod(tenorVol.first), tenorVol.second);
    }
    mkt.addVolCurve_2(volname_2, volCurve_2);

    // Read and add curve data
    string curvename_1 = "USD-SOFR";
    std::string curvePath_1 = basePath + "usd_sofr_20240601.txt";
    vector< pair<string, double> >curveData_1;

    readFromFile(curvePath_1,curvename_1, curveData_1);
    RateCurve usdSofr_1(curvename_1);
    for (auto& tenorRate : curveData_1) {
        usdSofr_1.addRate(valueDate + stod(tenorRate.first), tenorRate.second);
    }
    mkt.addCurve_1(curvename_1, usdSofr_1);

    // Read and add curve data
    string curvename_2 = "USD-SOFR";
    std::string curvePath_2 = basePath + "usd_sofr_20240602.txt";
    vector< pair<string, double> >curveData_2;

    readFromFile(curvePath_2,curvename_2, curveData_2);
    RateCurve usdSofr_2(curvename_2);
    for (auto& tenorRate : curveData_2) {
        usdSofr_2.addRate(valueDate + stod(tenorRate.first), tenorRate.second);
    }
    mkt.addCurve_2(curvename_2, usdSofr_2);

    mkt.Print();


    //task 2, create a portfolio of bond, swap, european option, american option
    vector<std::unique_ptr<Trade>> myPortfolio;
    std::string portfolioPath = basePath + "portfolio.txt";
    auto bFactory = std::make_unique<BondFactory>();
    auto sFactory = std::make_unique<SwapFactory>();
    auto eFactory = std::make_unique<EurOptFactory>();
    auto aFactory = std::make_unique<AmericanOptFactory>();

    std::ifstream file(portfolioPath)   ;
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << portfolioPath << std::endl;
        return 0;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#' || line[0] == 'I') {
                continue; // Skip comments or empty lines
            }
        cout << line << endl;
        myPortfolio.push_back(std::move(mkt.addPortfolio(line)));
        cout << "Added new trade from txt to myPortfolio trade vector" << endl;
        cout << endl;
    }
    file.close();

  //task 3, creat a pricer and price the portfolio, output the pricing result of each deal 
  //3.1 compute the NPV of deal as of market date 1
  //3.2 compute the NPV of deal as of market date 2, and then compute the daily Pnl for each deal uisng NPV(date2) - NPV (date1), and output the result in file
  auto pricer = new CRRBinomialTreePricer(100);
  for (size_t i = 0; i<myPortfolio.size(); i++) {
      auto& trade = myPortfolio[i];
      double pv = pricer->Price(mkt, std::move(trade));
      cout << pv << endl;
  }

  //task 4, compute the Greeks of DV01 [Vector], and Vega risk as of market date 1
  // 4.1 compute risk using risk engine
  // 4.2 use idea of multi-threading
  // analyzing the risk of PnL and risk of your portfolio and come out some simple strategy of a) square off the risk, b) maximize the PnL


  //final
  cout << "Project build successfully!" << endl;
  return 0;

}
