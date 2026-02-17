#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <logging.h>
#include <OpenXLSX.hpp>
#include <vector>

/**
 * @brief Class responsible for handling availability data for individual entities.
 *
 * This class handles availability data for entities, from reading input data
 * to process and provide data access.
 *
 * Responsibilities:
 * - Reading the availability data from XLSX input files for entities.
 * - Showing the availability data for debug purposes.
 * - Providing set of data accessing functions.
 */
class Availability
{
  public:
    /**
     * @brief Class responsible for storing availability reading settings.
     * 
     */
    class availabilitySettings
    {
      public:
        std::string initialsColumn; ///< Column in which entity initials should be found
        int startColumn; ///< Column in which start of availability matrix is located
        int endColumn; ///< Column in which end of availability matrix is located
        int minInitialsLength; ///< Minimum length of initials
        int maxInitialsLength; ///< Maximum length of initials
        int maxNoOfAvailableUnits; ///< Maximum number of available units
        int maxNoOfAvailableDays; ///< Maximum number of available days
        int availabilityMatrixRowOffset; ///< Row offset between availability matrixes
        int initialsToAvailabilityRowOffset; ///< Row offset between initials and availability matrix
    };
    /**
     * @brief Construct a new Availability object
     *
     * @param[in] wks Worksheet providing access to availability data stored in an XLSX file.
     * @param[in,out] initialsRowPointer Current row index used during matrix parsing.
     * @param[in,out] logger Logger instance for reporting processing details.
     */
    Availability(OpenXLSX::XLWorksheet& wks,
                 uint32_t& initialsRowPointer,
                 Logging& logger);

    /**
     * @brief Funtion to read an availability of the entity from XLSX input file
     * 
     * @return int 1 if availability matrix was read, 0 if not
     */
    int readAvailability();

    /**
     * @brief Funtion to find initials in defined cell and verify their length
     * 
     * @return int 1 if initials were found and their length is appropirate
     */
    int findAndCheckInitials();

    /**
     * @brief Function to read an availability matrix from XLSX input file
     * 
     * @return int 1 if matrix was successfully read
     */
    int readAvailabilityMatrix();

    /**
     * @brief Funtion to show availability matrix of the entity
     * 
     */
    void showAvailability();

    /**
     * @brief Function to count available units for the entity
     * 
     * @return amount of available units
     */
    int countAvailabilityUnits();

    /**
     * @brief Get the of the availablity entity
     * 
     * @return Name of the entity
     */
    std::string getName();

    /**
     * @brief Get the Availability matrix object
     * 
     * @return const std::vector<std::vector<int>>& 
     */
    const std::vector<std::vector<int>>& getAvailabilityVector() const;

    /**
     * @brief Function to check entity availability for the particular entity
     * 
     * @param[in] row Row of the entity to check availability
     * @param[in] col Column of the entity to check availability
     * @return true if the entity is available, false if not
     */
    bool isAvailable(int row, int col);

    /**
     * @brief Funtion to update the entity on the stored availability matrix
     * 
     * @param row Row of the entity to update availability
     * @param col Column of the entity to update availability
     * @param availabilityStatus Availability status to be updated
     */
    void storeAvailability(int row, int col, int availabilityStatus);

  protected:

    OpenXLSX::XLWorksheet& wks_; ///< OpenXLSX::XLWorksheet object using to read an availability from XLSX file
    Logging& logger_; ///< logger object instance
    uint32_t& initialsRowPointer_; ///< Row pointer using to for availability matrixes reading
    availabilitySettings availabilitySettings_; ///< Object of stored availability reading settings
    std::string initials_; ///< Read entity initials
    std::vector<std::vector<int>> availabilityMatrix_; ///< Read entity availability matrix
};
