#include "school.h"

school::school()
{

}

int school::readTeacherAvailabilityFromFile(string inputFilePath)
{
    //Create temporary teacher object
    teacher tempTeacher;

    //Found techaer counter 
    int teacherCounter = 0;
    
    //Read teacher availability in loop
    while (tempTeacher.readAvailability(inputFilePath))
    {
        teacherCounter++;
        teachers.push_back(tempTeacher);
        cout << "\nFound teacher No.: " << teacherCounter << endl;
    }

    cout << "\nStopped searching teachers: " << teacherCounter << endl;

    return teacherCounter;
}
void school::showTeachersAvailability()
{
    for (int i = 0; i < teachers.size(); i++) 
    {
        teachers[i].showAvailability();
    }

}