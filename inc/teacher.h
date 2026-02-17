 #pragma once 

 #include <iostream>
 #include <string>
 #include "availability.h"
#include "programsettings.h"

using namespace std;

/**
 * @brief Class responsible for handling teacher data
 *
 * This class extends the Availability base class by adding teacher-specific data processing
 * and providing access to the availability data of the teacher entity.
 *
 * Responsibilities:
 * - Orchestrates the department availability data
 * - Provide data needed for scheduling process
 */
class teacher : public Availability
{
  public:
    /**
    * @brief Construct a new teacher object
    *
    * @param[in] wks Worksheet providing access to availability data stored in an XLSX file.
    * @param[in,out] initialsRowPointer Current row index used during matrix parsing.
    * @param[in,out] logger Logger instance for reporting processing details.
    */
    teacher(OpenXLSX::XLWorksheet& wks,
            uint32_t& initialsRowPointer,
            Logging& logger);
};



