#include "teacher.h"

int teacher::readAvailability()
{
    
    OpenXLSX::XLDocument doc;

    //Create doc document
    doc.create(settings); 

    //Create worksheet
    XLWorksheet wks = doc.workbook().worksheet(1);

    auto cell = worksheet.cell("B2"); 

    //Closing XLSX document
    doc.close();
    
    return 0;
}

