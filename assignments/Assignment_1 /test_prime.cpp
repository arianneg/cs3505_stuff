/* Arianne Grimes u0830364*/
#include <iostream>   /*library for cin and cout*/
#include <cstdlib>   /*library for atoi*/

using namespace std;

/* test_prime main function. One integer argument from the command line is expected. Determines if the 
 * given integer is prime or composite. If an argument is not given, or if more than one argument is given,
 * an error message is returned*/
int main (int argc, char* argv[]){
  
  /* check if more than one argument is given*/
  if (argc > 2){
    cout<<"Too many arguments. Enter one integer at a time to check if it is prime."<<endl;
    return 0;
  }

  /* check if no arguments are given*/
  if(argc < 2){
    cout<<"No argument given. Enter an integer to check if it is prime."<<endl;
    return 0;
  }
  
  /*parse argument as an integer*/
  int number = atoi(argv[1]);

  /* no integers < 2 are prime */
  if(number < 2){
    cout<<"composite"<<endl;
    return 0;
  }

  /* loop through integers up to number. 
   *If number is evenly divisible by any integer >= 2, it is not prime.*/
  for(int i = 2; i < number; i++){
    if(number % i == 0){
      cout<<"composite"<<endl;
      return 0;
    }
  }

  /* No divisor was found, number is prime*/
  cout<<"prime"<<endl;
  return 0;


  }
