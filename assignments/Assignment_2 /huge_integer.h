// Arianne Grimes 

#ifndef HUGE_INTEGER
#define HUGE_INTEGER
#include<string>

// represents a class that allows for the storing of, and 
// basic math funtionality on, unsigned integers of arbirary size 
class huge_integer{

public:

  //default constructor. Initializes to "0"
  huge_integer();

  //string constructor. Constructs the huge_integer to be the number represented 
  //by the given string 
  huge_integer(std::string s);

  //copy constructor. Constructs the huge_integer to be the number represnted 
  //by the string within the given huge_integer
  huge_integer(const huge_integer & hi);

  //accessor method. Returns the string representation of 'this' huge_integer
  std::string get_value()const ;
  
  //= overload. Sets the lhs huge_integer to equal the rhs huge_integer
  huge_integer & operator = (const huge_integer & rhs);

  //+ overload. Returns a huge_integer which represents lhs + rhs
  huge_integer operator + (const huge_integer & rhs) const;

  //- overload. Returns a huge_integer which represents lhs - rhs
  huge_integer operator - (const huge_integer & rhs) const;

  //* overload. Returns a huge_integer which represents lhs * rhs
  huge_integer operator * (const huge_integer & rhs) const;

  // / overload. Returns a huge_integer which represents lhs / rhs
  huge_integer operator / (const huge_integer & rhs) const;

  //% overload. Returns a huge_integer which represents lhs % rhs
  huge_integer operator % (const huge_integer & rhs) const;

 private :

  //private string that stores the integer itself, no non integer characters
  std::string number;

  //add helper method. Takes in two constant strings as arguments and 
  //returns a string that represents their sum (left + right).
  //Does not change 'this'.
  std::string add(const std::string left, const std::string right)const;

  //subtract helper method. Takes in two constant strings as arguments and 
  //returns a string that represents their difference (left - right).
  //Does not change 'this'. 
  std::string subtract(const std::string left, const std::string right)const;

  //multiply helper method. Takes in two constant strings as arguments and 
  //returns a string that represents their product (left * right).
  //Does not change 'this'.
  std::string multiply(const std::string left, const std::string right)const;

  //divide helper method. Takes in two constant strings as arguments and 
  //returns a string that represents their quotient (left / right).
  //Does not change 'this'.
  std::string divide(const std::string left, const std::string right)const;

  //mod helper method. Takes in two constant strings as arguments and 
  //returns a string that represents their mod (left % right).
  //Does not change 'this'.
  std::string mod(const std::string left, const std::string right)const;

  //helper method. Takes in two constant strings as arguments and determines if
  //the first string argument given is bigger than the second. Returns true 
  //if first string argument is bigger, false if it is not.
  //Does not change 'this'.
  bool firstIsBiggerThanSecond(const std::string left, const std::string right)const;

  //helper method. Takes in one string argument and returns it 
  //with all the leading zeros removed, leaving at least one digit.
  //Does not change 'this'. 
  std::string stripZeros(std::string s)const; 
};
#endif
