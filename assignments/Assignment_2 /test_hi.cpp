

#include <iostream>
#include "huge_integer.h"
#include <string>

using namespace std;
int main(){

  string oh = "100";
  string ft = "000052";
  string nn = "99";
  string sn= "79";
  string fe = "1032144985";
  string ni = "5006901369";
  string onethousand = "444";
  string ten = "222";
  string nine = "156154285";
  string five = "15369";

  huge_integer a;        //0
  huge_integer b(oh);    //100
  huge_integer c(ft);    //52
  huge_integer d(nn);    //99
  huge_integer e(b);     //100
  huge_integer f(sn);    //79
  huge_integer g(fe);    //ends in 5
  huge_integer h(ni);     //ends in 9
  huge_integer nine9 (nine);
  huge_integer five5 (five);

  cout<< "expected: 0 actual:"<<a.get_value()<<endl;
  cout<< "expected: 100 actual: "<<b.get_value()<<endl;
  cout<< "expected: 52 actual: "<<c.get_value()<<endl;
  cout<< "expected: 99 actual: "<<d.get_value()<<endl;
  cout<< "expected: 100 actual: "<<e.get_value()<<endl;

  huge_integer added1 ;
  huge_integer added2 ;
  huge_integer added3 ;
  huge_integer added4 ;

  huge_integer mult1 ;
  huge_integer mult2 ;
  huge_integer mult3;
  huge_integer mult4;
  huge_integer mult5;

  huge_integer sub1;
  huge_integer sub2;
  huge_integer sub3;

  huge_integer div1;
  huge_integer div2;
  huge_integer div3;
  huge_integer div4;
  huge_integer div5; 

  huge_integer mod1;
  huge_integer mod2; 
  huge_integer mod3;

  added1= b+c;
  added2= c+d;
  added3= g+h;
  added4= h+g;

  mult1 = b*c;
  mult2= c*d;
  mult3 = a*b;
  mult4 = b*a;
  mult5 = g*h;


  sub1 = b-c;
  sub2 = e-d;
  sub3 = h-g;

  div1= b/a;
  div2= b/c;
  div3 = h/g ;//=4
  div4 = g/f ;
   
  mod1 = b%c; 
  mod2 = g%f;
  mod3 = nine9%five5;

  cout<<"expected: 152 actual: "<<added1.get_value()<<endl;
  cout<<"expected: 151 actual: "<<added2.get_value()<<endl;
  cout<<"expected: 6039046354: actual: "<<added3.get_value()<<endl;
  cout<<"expected: 6039046354: actual: "<<added4.get_value()<<endl;

  cout<<"expected: 5200 actual: "<<mult1.get_value()<<endl;
  cout<<"expected: 5148 actual: "<<mult2.get_value()<<endl;
  cout<<"expected: 0 actual: "<<mult3.get_value()<<endl;
  cout<<"expected: 0 actual: "<<mult4.get_value()<<endl;
  cout<<"expected: 5167848138402984465 actual: "<<mult5.get_value()<<endl;

  cout<<"expected: 48 actual: "<<sub1.get_value()<<endl;
  cout<<"expected: 1 actual: "<<sub2.get_value()<<endl;
  cout<<"expected: 3974756384 actual: "<<sub3.get_value()<<endl;

  cout<<"expected: 0 actual: "<<div1.get_value()<<endl;
  cout<<"expected: 1 actual: "<<div2.get_value()<<endl;
  cout<<"expected: 4 actual: "<<div3.get_value()<<endl;
  cout<<"expected: 13065126 actual: "<<div4.get_value()<<endl;

  cout<<"expected: 48 actual: "<<mod1.get_value()<<endl;
  cout<<"expected: 31 actual: "<<mod2.get_value()<<endl;
  cout<<"expected: 5245 actual: "<<mod3.get_value()<<endl;

  return 0;
}
