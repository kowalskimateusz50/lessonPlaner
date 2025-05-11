 #pragma once 

 #include <iostream>
 #include <string>
 #include <OpenXLSX.hpp>
 #include <vector>
 #include "programSettings.h"

using namespace std;
using namespace OpenXLSX;

enum class availableDays 
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
    static uint teacherInitialsReadLine;
    string initials;
    vector<vector<int>> availabilityMatrix;

public:
    teacher();
    int readAvailability(string inputFilePath);
    int findAndCheckInitials(const OpenXLSX::XLWorksheet& wks);
    int readAvailabilityMatrix(const OpenXLSX::XLWorksheet& wks);

};



