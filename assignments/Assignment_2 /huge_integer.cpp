#include "huge_integer.h"
#include <iostream>

#include<sstream>
using namespace std;


//default constructor
huge_integer::huge_integer()
{
  this->number = "0";
}

//string input constructor
huge_integer::huge_integer(string s)
{
  string noZerosString = stripZeros(s);
  this->number = noZerosString;
}

//copy constructor
huge_integer::huge_integer(const huge_integer & rhs)
{
  string noZerosString = stripZeros(rhs.number);
  this->number = noZerosString;
}

//accessor 
std::string huge_integer:: get_value() const
{
  return this-> number;
}

//= overload 
huge_integer & huge_integer::operator=(const huge_integer & rhs)
{
  this->number = rhs.number;
  return *this;
}

//+overload. Uses add helper method 
huge_integer huge_integer::operator+(const huge_integer & rhs)const
{
   huge_integer temp(add(this->number, rhs.number));
   return temp;
}

//-overload. Uses subtract helper method 
huge_integer huge_integer::operator-(const huge_integer & rhs)const
{
  huge_integer temp(subtract(this->number, rhs.number));
  return temp;
}

//* overload. Uses multiply helper method 
huge_integer huge_integer::operator*(const huge_integer & rhs)const
{
  huge_integer temp(multiply(this->number, rhs.number));
  return temp;
}

///overload. Uses division helper method
huge_integer huge_integer::operator/(const huge_integer & rhs)const
{
  huge_integer temp(divide(this->number, rhs.number));
  return temp;
}

//%overload. Uses mod helper method
huge_integer huge_integer::operator%(const huge_integer & rhs)const
{
  huge_integer temp(mod(this->number, rhs.number));
  return temp;
}

/*
 * Adds two integers stored in strings, building a string result.
 */
string huge_integer::add (const string left, const string right)const
{
  // Build up a string to contain the result.
  string result = "";

  // Work right to left.
  int left_pos  = left.length()  - 1;
  int right_pos = right.length() - 1;

  //carry stored here 
  int carry = 0;

  // Loop, adding columns until no more digits remain.
  while (left_pos >= 0 || right_pos >= 0 || carry > 0)
  {
    // Get the digit from each string, or 0 if none.
    int left_digit;
    int right_digit;

    //check if we have reached the beginning of the left string 
    //if not, get the next digit 
    if(left_pos >= 0)
      {
        left_digit = left[left_pos--] - '0';
      }
    //if we reached the end, next digit is 0
    else
      {
	left_digit = 0;
      }

    //check if we have reached the beginning of the right string 
    //if not, get the next digit 
    if(right_pos >= 0)
      {
	right_digit = right[right_pos--] - '0';
      }
    //if we reached the end, next digit is 0
    else
      {
	right_digit = 0;
      }

    // Calcuate the sum of the digits.
    int sum = left_digit + right_digit + carry;

    //calculate carry 
    carry = sum / 10;

    //calculate the one's place of the sum, ten's place goes in carry
    sum = sum % 10;

    // Put the sum into the new string (at the left side)
    result.insert (0, 1, (char)(sum+'0') );    
  }
  //return the new string 
  return result;
}


// Strip out any leading 0's from our result (but leave at least one digit).
string huge_integer::stripZeros(string result)const
{
  //loop though digits, erasing leading zeros
  while (result.length() > 1 && result[0] == '0'){
    result.erase(0, 1);}
  
  return result;
}

// subtraction helper method. Subtracts two integers stored in strings,
// builds a new result string  
string huge_integer::subtract(const string left, const string right)const
{
  //check that minuend is greater than the subtrahend
  //if it is not, return 0;
  if(!firstIsBiggerThanSecond(left, right)){
    return "0";
  }
  
  // new result string. difference will be stored here
  string result = "";

  //go from right to left 
  int left_pos  = left.length()  - 1;
  int right_pos = right.length() - 1;

  //keeps track of whether or not we had to borrow
  bool borrow = false;

  // loop over the digits of the minuend 
  while(left_pos >=0)
  {
    //get the minuend digit
    int left_digit = left[left_pos--] - '0';

    int right_digit;

    //if we have reached the beginning of the subtrahend
    //the next subtrahend digit is 0
    if(right_pos < 0){
      right_digit = 0;
    }
     //get the subtrahend digit
    else{
      right_digit = right[right_pos--] - '0';
    }

    //if we had to borrow in the previous loop
    //add 1 to the subtrahend digit
    if(borrow){
      right_digit = right_digit + 1;
    }

    // if left digit is less than right digit we need to borrow,
    //add 10 to the minuend digit
    if(left_digit < right_digit)
    {
      left_digit = 10 + left_digit; //add ten to left digit
      borrow = true;
    }
    //we did not need to borrow 
    else{
      borrow = false;
    }
    //calculate the difference, insert into result string 
     int difference = left_digit - right_digit;
     result.insert (0, 1, (char)(difference + '0')); 
  }

  //strip out leading zeros from result and return
  string resultNoZeros = stripZeros(result);
  return resultNoZeros;

}
/*
 * Multiplies two integers stored in strings, building a string result.
 * The algorithm is long multiplication, starting with the most significant
 * digit of the multiplier.
 */
string huge_integer::multiply (const string left, const string right) const
{
  //string result to be built
   string result = "0";

   // move from left to right in the multiplier
    int right_pos = 0;

    //loop over the digits in the multiplier
    while (right_pos < right.length())
    {
      // Multiply the product by 10. As is done in long multiplication 
      //when moving from one column of the multiplier to the next 
      result.append("0");  

      //get the next digit in the multiplier
      int right_digit = right[right_pos++] - '0';

      //add up the current and previous products
      for (int i = 0; i < right_digit; i++){
        result = add(result, left);
      }
    }
    //strip out any leading zeros and return 
    string resultNoZeros =  stripZeros(result); 
    return resultNoZeros;
}

//divides the two integers stored in strings, building a string quotient
//The algorithm is based on long division, starting with the most significant 
//digit of the dividend 
string huge_integer::divide(const string left, const string right)const{
  string quotient = "";
  
  //check for division by zero 
  if(right.compare("0")==0){
    return quotient = "0";
  }

  //check for a bigger divisor than dividend
  //if the divisor is bigger than the dividend, the quotient cannot be >= 1
  if(!firstIsBiggerThanSecond(left, right)){
    return quotient = "0";
  }

  //we move from left to right along the dividend 
  //get the first digit of the dividend 
  string remainder = left.substr(0, 1);

  //loop over each digit in the dividend 
  for(int left_loc = 1; left_loc  <=  left.length(); left_loc++){

     // loop over digits 0 through 9 to find the greatest 
     //multiple of the divisor that is not greater than the dividend digit(s)
     for(int i = 0; i < 10; i++){
       
       //turn i into a string 
       stringstream stringTemp;
       stringTemp<<i;
       string istr = stringTemp.str();
       
       //multiply i and the divisor 
       string scaledDivisor = multiply(right, istr);

       //subtract the multiple of the divisor from the dividend digit(s)
       string testScale  = subtract(remainder, scaledDivisor);

       // if the divisor is greater than the difference computed on the previous line 
       if(firstIsBiggerThanSecond(right, testScale)){

	 //i is the next digit of the quotient 
         quotient = quotient + istr;

	 //check if we have reached the end of the dividend 
	 if(left_loc < left.length()){

	   //if we have not reached the end of the dividend,
	   //concatinate the next digit of the dividend onto the difference 
	   //computed above and store as the new dividend digit(s)
	   remainder = testScale + left.substr((size_t)left_loc, 1);
	   
	   //strip off the leading zeros 
	   remainder = stripZeros(remainder);

	 }
	 //break out of the inner for loop because the multiple of the divisor 
	 //has been found 
         break;
       }
    }
  }
  //strip off the leading zeros from the quotient and return 
  string quotientNoZeros = stripZeros(quotient);
  return quotientNoZeros;
}

//computes the mod of the two integers stored in strings, building a string remainder.
//The algorithm is based on long division, starting with the most significant 
//digit of the dividend 
string huge_integer::mod(const string left, const string right)const{
  
  //check for division by zero 
  if(right.compare("0")==0){
    return "0";
  }

  //check for a bigger divisor than dividend. 
  //if the divisor os bigger than the dividend, the mod is the dividend 
  if(!firstIsBiggerThanSecond(left, right)){
    return left;
  }

  //we move from left to right along the dividend 
  //get the first digit of the dividend 
  string remainder = left.substr(0, 1);

  //remainder string that will produce the final result 
  string testScale ; 

   //loop over each digit in the dividend 
  for(int left_loc = 1; left_loc  <=  left.length(); left_loc++){

     // loop over digits 0 through 9 to find the greatest 
     //multiple of the divisor that is not greater than the dividend digit(s)
     for(int i = 0; i < 10; i++){
       
       //turn i into a string 
       stringstream ss;
       ss<<i;
       string istr = ss.str();
       
        //multiply i and the divisor 
       string scaledDivisor = multiply(right, istr);

       //subtract the multiple of the divisor from the dividend digit(s)
       //this will eventually yeild the remainder
       testScale  = subtract(remainder, scaledDivisor);

        // if the divisor is greater than the difference computed on the previous line 
       if(firstIsBiggerThanSecond(right, testScale)){

	  //check if we have reached the end of the dividend 
	 if(left_loc < left.length()){

	    //if we have not reached the end of the dividend,
	   //concatinate the next digit of the dividend onto the difference 
	   //computed above and store as the new dividend digit(s)
	   remainder = testScale + left.substr((size_t)left_loc, 1);

	   //strip off the leading zeros 
	   remainder = stripZeros(remainder);

	 }
	 //break out of the inner for loop because the multiple of the divisor 
	 //has been found 
         break;
       }
    }
  }
    //strip off the leading zeros from the remainder and return 
  string modNoZeros = stripZeros(testScale);
  return modNoZeros;

}
//helper method. returns true if the first argument is bigger than the second. false otherwise.
bool huge_integer::firstIsBiggerThanSecond(const string lhs, const string rhs)const
{
  //strip off leading zeros 
  string left = stripZeros(lhs);
  string right = stripZeros(rhs);

  //if the length of the first string is bigger 
  // than the length of the second, then the 
  //first integer is bigger
  if (left.length()>right.length())
  {
    return true;
  }
   //if the length of the first string is smaller 
  // than the length of the second, then the 
  //first integer is not bigger
  if(left.length()<right.length())
  {
    return false;
  }
  //if their lengths are the same
  if(left.length()==right.length())
  {
    //loop over the digits 
    for(int i = 0; i < left.length(); i++)
    {
      //move from left to right 
      int left_digit = left[i] - '0';
      int right_digit = right[i] - '0';
      
      // the first string to have a larger digit than
      // the other is the larger string 
      if (left_digit > right_digit)
      {
	return true;
      }  
       if (left_digit < right_digit)
      {
	return false;
      }  
    }
  }
  return false;
}

