

#include "Cell.h"

using namespace std;
Cell::Cell(string Name, string Contents){
  cellName= Name;
  cellContents = Contents;



}

string Cell::GetCellName(){

  return cellName;


}

string Cell::GetCellContents(){

  return cellContents;
}
