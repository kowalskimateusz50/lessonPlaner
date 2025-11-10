#include "school.h"

school::school()
{
    
  
}

/**
 * @brief Funtion to read teacher availability
 *
 * @param[in] wks reference to excel works sheet document
 *
 * @return amount of read teachers
 */
int school::readTeachersAvailability(OpenXLSX::XLWorksheet& wks)
{
    teacher readTeacher;
    int teacherCounter = 0;
    uint rowPointer = 2;

    while (readTeacher.readAvailability(wks, rowPointer))
    {
        teacherCounter++;
        teachers.push_back(readTeacher);
        cout << "\nFound teacher No.: " << teacherCounter << endl;
    }

    cout << "\nStopped searching teachers: " << teacherCounter << endl;
    return teacherCounter;
}
/**
 * @brief funtion to display teachers availability
 * 
 */
void school::showTeachersAvailability()
{
    for (int i = 0; i < teachers.size(); i++)
    {
        teachers[i].showAvailability();
    }
}

/**
 * @brief Funtion to read departments availability
 *
 * @param[in] wks reference to excel works sheet document
 *
 * @return amount of read departments
 */
int school::readDepartmentsAvailability(OpenXLSX::XLWorksheet& wks)
{
    department readDepartment;
    int departmentCounter = 0;
    uint rowPointer = 2;

    while (readDepartment.readAvailability(wks, rowPointer))
    {
        departmentCounter++;
        departments.push_back(readDepartment);
        cout << "\nFound department No.: " << departmentCounter << endl;
    }

    cout << "\nStopped searching departments: " << departmentCounter << endl;
    return departmentCounter;
}

/**
 * @brief funtion to display departments availability
 * 
 */
void school::showDepartmentsAvailability()
{
    for (int i = 0; i < teachers.size(); i++)
    {
        departments[i].showAvailability();
    }
}