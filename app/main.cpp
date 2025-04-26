#include <OpenXLSX.hpp>
#include <iostream>

using namespace std;
using namespace OpenXLSX;

int main() 
{

    OpenXLSX::XLDocument doc;

    //Create doc document
    doc.create("example.xlsx"); 

    //Create worksheet
    XLWorksheet wks = doc.workbook().worksheet(1);

    wks.cell("A1") = "Hello";
    wks.cell("B1") = "World";

    //Closing XLSX document
    doc.save();
    doc.close();

    return 0;
}