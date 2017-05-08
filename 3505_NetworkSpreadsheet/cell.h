#ifdef CELLSPREADSHEET_H
#define CELLSPREADSHEET_H

#include <string>


class Cell{
 private:

  std::string cellName;
  std::string cellContents;

 public:


  Cell(std::string cellName, std::string cellContents);
  
  std::string GetCellName();

  std::string GetCellContents();


};



#endif
