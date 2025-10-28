#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "teacher.h"
#include "programSettings.h"

using namespace std;

/**
 * @brief Class which describes department
 * 
 * 
 * 
 */

class department
{
    public:

      uint year;
      string name;
      vector<string> assignedTeachers;
      department();
      

};

