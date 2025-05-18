#pragma once

#include <iostream>
#include <vector>
#include "department.h"
#include "teacher.h"

using namespace std;

class school 
{
    private:
     vector <teacher> teachers;

    public:
    school();

    //Methods
    int readTeacherAvailabilityFromFile(string inputFilePath);
    void showTeachersAvailability();
};

