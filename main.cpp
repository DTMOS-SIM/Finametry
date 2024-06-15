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
  valueDate.year = now_->tm_mday;

  Market mkt = Market(valueDate);
  /*
  load data from file and update market object with data
  */


  //task 2, create a portfolio of bond, swap, european option, american option
  vector<std::unique_ptr<Trade>> myPortfolio;
  
  auto bFactory = std::make_unique<BondFactory>();
  auto sFactory = std::make_unique<SwapFactory>();
  auto eFactory = std::make_unique<EurOptFactory>();
  auto aFactory = std::make_unique<AmericanOptFactory>();
  auto bond = bFactory->createTrade("usd-gov",Date(2024, 1, 1), Date(2034, 1, 1),1000000, 0.035, 0.6, OptionType::None);
  auto swap = sFactory->createTrade("usd-sofr",Date(2024, 1, 1), Date(2034, 1, 1),-1000000, 0.03, 1.0, OptionType::None);
  auto eCall = eFactory->createTrade("appl", Date(2024, 1, 1), Date(2025, 1, 1), 10000, 530, 0,OptionType::Call);
  auto aPut = aFactory->createTrade("appl", Date(2024, 1, 1), Date(2026, 1, 1), 10000, 525, 0,OptionType::Put);

  myPortfolio.push_back(std::move(bond));
  myPortfolio.push_back(std::move(swap));
  myPortfolio.push_back(std::move(eCall));
  myPortfolio.push_back(std::move(aPut));

  //task 3, creat a pricer and price the portfolio, output the pricing result of each deal 
  //3.1 compute the NPV of deal as of market date 1
  //3.2 compute the NPV of deal as of market date 2, and then compute the daily Pnl for each deal uisng NPV(date2) - NPV (date1), and output the result in file
  auto pricer = new CRRBinomialTreePricer(100);
  for (size_t i = 0; i<myPortfolio.size(); i++) {
    auto& trade = myPortfolio[i];
    double pv = pricer->Price(mkt, std::move(trade));
    //log pv details out in a file
  }

  //task 4, compute the Greeks of DV01 [Vector], and Vega risk as of market date 1
  // 4.1 compute risk using risk engine
  // 4.2 use idea of multi-threading
  // analyzing the risk of PnL and risk of your portfolio and come out some simple strategy of a) square off the risk, b) maximize the PnL


  //final
  cout << "Project build successfully!" << endl;
  return 0;

}
