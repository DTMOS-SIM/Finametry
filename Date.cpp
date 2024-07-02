#include "Date.h"
#include <sstream>

//this function return the year franction between 2 date object


Date operator+(const Date& d, double yearFrac)
{
  int numYear = yearFrac / 1.0;
  yearFrac = yearFrac - numYear / 1.0;
  int numMonth = yearFrac * 12;
  yearFrac = yearFrac - numMonth / 12.0;
  int numDays = yearFrac * 365;
  int _day = d.day + numDays;
  int _month = d.month + numMonth;
  int _year = d.year + numYear;

  switch (d.month) {
  case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      if (_day > 31) {
        _day = _day - 31;
        _month++;
      }
  case 4: case 6: case 9: case 11:
      if (_day > 30) {
        _day = _day - 30;
        _month++;
      }
  case 2:
    if ((d.year % 400 == 0) || ((d.year % 100 != 0) && (d.year % 4 == 0))) {
      if (_day > 29) {
        _day = _day - 29;
        _month++;
      }
    }
    else {
      if (_day > 28) {
        _day = _day - 28;
        _month++;
      }
    }
  }

  if (_month > 12) {
    _month = _month - 12;
    _year++;
  }

  return Date(_year, _month, _day);
}

double operator-(const Date& d1, const Date& d2)
{
  int yearDiff = d1.year - d2.year;
  int monthDiff = (d1.month - d2.month);
  int dayDiff = d1.day - d2.day;
  return yearDiff + monthDiff / 12.0 + dayDiff / 360.0;
};

bool operator>(const Date& d1, const Date& d2)
{
  double gap = d1 - d2;
  if (gap > 0)
    return true;
  else
    return false;
};

bool operator<(const Date& d1, const Date& d2)
{
  double gap = d1 - d2;
  if (gap < 0)
    return true;
  else
    return false;
};

bool operator<=(const Date& d1, const Date& d2)
{
  return (d1 - d2) <= 0;
}
bool operator>=(const Date& d1, const Date& d2)
{
  return (d1 - d2) >= 0;
}

bool operator==(const Date& d1, const Date& d2)
{
  double gap = d1 - d2;
  if (gap == 0)
    return true;
  else
    return false;
};

bool operator!=(const Date& d1, const Date& d2)
{
  return (d1 - d2) != 0;
}

std::ostream& operator<<(std::ostream& os, const Date& d)
{
  os << d.year << " " << d.month << " " << d.day << std::endl;
  return os;
}

std::istream& operator>>(std::istream& is, Date& d)
{
  is >> d.year >> d.month >> d.day;
  return is;
}

Date dateAddTenor(const Date& start, const string& tenorStr)
{
  Date newdate = start;
  auto tenorUnit = tenorStr.back();
  int numUnit = stoi(tenorStr.substr(0, tenorStr.size()-1));
  
  if (to_lower(tenorStr) == "on" || to_lower(tenorStr) == "o/n"){
    newdate.day +=1;
    if (newdate.day > 30){
      newdate.month +=1;
      newdate.day=1;
    }
  }
  else if (tenorUnit == 'W'){
    int newDay = newdate.day + numUnit * 7;
    if (newDay > 30) {
      newdate.month +=1;
      newDay = newDay-30;
      newdate.day=newDay;
    }
  }
  else if(tenorUnit == 'M'){
    int newMonth = newdate.month + numUnit;
    if (newMonth > 12) {
      newdate.year +=1;
      newMonth = newMonth-12;
      newdate.month=newMonth;
    }
  }
  else if(tenorUnit =='Y'){
    newdate.year += numUnit;
  }
  else
    throw std::runtime_error("Error: found unsupported tenor: "+ tenorStr);
  
  return newdate;
}

Date stringToDate(const std::string& DateStr)
{
  // cout << "triggered date conversion" << endl;
  Date date;
  std::stringstream ss(DateStr);
  std::string token;
  std::vector<std::string> tokens;

  // Use getline with '-' as delimiter
  while (std::getline(ss, token, '-')) {
    tokens.push_back(token);
  }

  if (tokens.size() == 3)
  {
    date.year = stoi(tokens[0]);
    date.month = stoi(tokens[1]);
    date.day = stoi(tokens[2]);
  } else
  {
    std::time_t t = std::time(nullptr);
    auto now_ = std::localtime(&t);
    date.year = now_->tm_year + 1900;
    date.month = now_->tm_mon + 1;
    date.day = now_->tm_mday;
  }


  return date;
}
