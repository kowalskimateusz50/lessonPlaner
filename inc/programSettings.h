#pragma once

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

namespace programConfig {
    constexpr int initialsReadLineBegining = 2;
    constexpr int initialsAvailabilityOffset = 1;
    constexpr int maxNoAvailabilityUnits = 10;
    constexpr int maxNoAvailabilityDays = 5;
    constexpr int availabilityMatrixOffset = 12;
    constexpr int initialsLength = 2;
}

enum class wksColumns {
    A = 1,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L
};

class programSettings
{
    private:
    string inputFilePath;

    public:
    programSettings();
    int readProgramSettings();
    string getInputFilePath();

};
