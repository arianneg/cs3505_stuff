/* Arianne Grimes u0830364*/

#include <iostream>   /*library for cin and cout*/

#include <iomanip>    /*library for setprecision and fixed*/

using namespace std;   /*standard namespace for cin and cout*/

/*main function. Prompts the user for a weight in pounds, converts the weight to kg, returns the weight in kg to the user rounding to two decimal places. */
int main (int argc, char* argv[]){

  /*prompt the user*/
  cout <<"Enter a weight in pounds." <<endl;

  /*get the weight in pounds*/
  double pounds;
  cin>>pounds;

  /*convert to kg*/
  double kg = pounds / 2.204;

  /*output kg rounded to two decimal places*/
  cout<< "Weight in kilograms:\n"<< setprecision(2) << fixed << kg <<endl;
 
  return 0;

}
