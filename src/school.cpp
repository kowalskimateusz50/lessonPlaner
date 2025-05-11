#include "school.h"

school::school()
{

}

int school::readTeacherAvailabilityFromFile(string inputFilePath)
{
    //Create temp teacher object
    teacher tempTeacher;

    //Found techaer counter 
    int teacherCounter = 0;
    
    //Read teacher availability in loop
    while (tempTeacher.readAvailability(inputFilePath))
    {
        teacherCounter++;
        cout << "\nFound teacher No.: " << teacherCounter << endl;
    }

    cout << "\nStopped searching teachers: " << teacherCounter << endl;   

    return teacherCounter;
}
