#pragma once

#include <iostream>
#include <vector>
#include "department.h"
#include "teacher.h"

using namespace std;

class school 
{
    private:
     vector <department> departments;

    public:
    school();

    //Methods
    int readTeacherAvailabilityFromFile(string inputFilePath);
};

