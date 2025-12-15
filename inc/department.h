#pragma once

#include <iostream>
#include <string>
#include <OpenXLSX.hpp>
#include "availability.h"
#include "programsettings.h"

/**
 * @brief Class which describes department
 * 
 * 
 *
 */
class department  : public Availability
{
  public:
    department(OpenXLSX::XLWorksheet& wks,
               uint& initialsRowPointer,
               Logging& logger);

};

