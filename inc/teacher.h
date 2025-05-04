 #pragma once 

 #include <iostream>
 #include <vector>

using namespace std;

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



