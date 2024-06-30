#ifndef PAYOFF_H
#define PAYOFF_H

#include "Types.h"

namespace PAYOFF {

  // Function declarations
  double VanillaOption(OptionType optType, double strike, double S);
  double CallSpread(double strike1, double strike2, double S);

}

#endif // PAYOFF_H
