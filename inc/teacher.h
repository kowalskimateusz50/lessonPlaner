 #pragma once 

 #include <iostream>
 #include <string>
 #include "availability.h"
#include "programsettings.h"

using namespace std;

/**
 * @brief Class which describes teacher
 * 
 *
 * 
 *
 */
class teacher : public Availability
{
  public:
    teacher(OpenXLSX::XLWorksheet& wks,
            uint32_t& initialsRowPointer,
            Logging& logger);
};



