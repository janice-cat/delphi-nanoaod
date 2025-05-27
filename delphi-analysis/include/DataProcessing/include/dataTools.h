#ifndef DATATOOLS_H
#define DATATOOLS_H

//c and cpp dependencies
#include <math.h>
#include <iostream>

inline int alephFloatDecSize = 6;

inline float reducedPrecision(float inVal, int newPrecision = alephFloatDecSize)
{
  int newValInt = inVal*std::pow(10, newPrecision+1);
  int roundVal = newValInt - ((int)(inVal*std::pow(10, newPrecision)))*10;
  newValInt -= roundVal;
  if(roundVal >= 5) newValInt += 10;
  float outVal = newValInt;
  return outVal/std::pow(10, newPrecision+1);
}

#endif
