#pragma once

#include <iostream>
#include <vector>
#include "department.h"

using namespace std;

class school 
{
    private:
     vector <department> departments;

    public:
    school();

    //Methods
    int readDeparmentsFromFile();
};

