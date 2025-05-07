 #pragma once 

 #include <iostream>
 #include <OpenXLSX.hpp>
 #include <vector>
 #include "programSettings.h"

using namespace std;
using namespace OpenXLSX;

enum class availableDay 
{
    poniedzialek,
    wtorek,
    sroda,
    czwartek,
    piatek
};


class teacher
{
    
    private:

    vector<vector<int>> availabilityMatrix;
    int readAvailability();

};



