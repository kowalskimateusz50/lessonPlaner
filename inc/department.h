#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <OpenXLSX.hpp>
#include "programSettings.h"

using namespace std;
using namespace OpenXLSX;
/**
 * @brief Class which describes department
 * 
 * 
 *
 */
class department
{
  public:
    department();
    int readAvailability(const OpenXLSX::XLWorksheet& wks, uint& initialsRowPointer);
    int findAndCheckInitials(const OpenXLSX::XLWorksheet& wks, uint& initialsRowPointer);
    int readAvailabilityMatrix(const OpenXLSX::XLWorksheet& wks, uint& initialsRowPointer);
    void showAvailability();

  private:
    avaiabilitySettings m_availabilitySettings;
    string m_initials;
    vector<vector<int>> m_availabilityMatrix;

};

