//arianne grimes

#include <iostream>
#include <fstream>
#include <set>
#include <iterator>
#include "wordset.h"
#include "node.h"
#include <vector>

using namespace std;

//forward declaration of helper methods 
bool vectors_match(vector<string> first, vector<string> second);
void print_vectors(vector<string> expected, vector<string> actual);
bool check_destruction();

int main ()
{
  /*
   *test copy constructor
   */
  {
    //create a wordset
    cs3505::wordset myset(5);
     myset.add("aaaa"); //hashes to 0
     myset.add("bbbb"); //hashes to 0
     myset.add("cccc"); //hasehs to 0
     myset.add("1");    //hashes to 4
     myset.add("12");   //hashes to 3
     myset.add("123");  //hashes to 2
   
     //use copy constructor to make a new wordset
     cs3505::wordset mysetToo(myset);
     
     //remove a few elements from old wordset
     myset.remove("aaaa");
     myset.remove("1");

     //create vector of expected elements 
     vector<string> expected ;
     expected.push_back("aaaa");
     expected.push_back("bbbb");
     expected.push_back("cccc");
     expected.push_back("123");
     expected.push_back("12");
     expected.push_back("1");

     //get vector of elements in the new wordset
     vector<string> actual (mysetToo.get_elements());

     cout<<"Test copy constructor:"<<endl;

     //print results 
    if(vectors_match(expected, actual)){
      cout<<"passed"<<endl;
    }
    else{
      cout<<"failed"<<endl;
      print_vectors(expected, actual);
    }
  }

  //check for memory leak
  if(!check_destruction){
    cout<<"memory leak detected"<<endl;
  }


  /*
   *test add and size
   */
  {
    cout<<"Test add and size:"<<endl;
    
    //create wordset
    cs3505::wordset myset(5);
     myset.add("aaaa"); //hashes to 0
     myset.add("bbbb"); //hashes to 0
     myset.add("cccc"); //hasehs to 0
     myset.add("1");    //hashes to 4
     myset.add("12");   //hashes to 3
     myset.add("123");  //hashes to 2
     
     //print results 
     if(myset.size() == 6){
      cout<<"passed"<<endl;
    }
    else{
      cout<<"failed"<<endl;
      cout<<"expected size: 6 actual size: "<<myset.size()<<endl;
    }
    
  }
  //check for memory leak
   if(!check_destruction){
    cout<<"memory leak detected"<<endl;
  }


   /*
    *add- no duplicates
    */
  {
    //create a wordset
    cs3505::wordset myset(5);
    myset.add("a");
    myset.add("a");
    
    cout<<"Test that duplicates are not added:"<<endl;

    //create a vector of expected elements
    vector<string> expected ; 
    expected.push_back("a");
    
    //get vector of elements in wordset
    vector<string> actual = myset.get_elements();

    //print results
    if(vectors_match(expected, actual)){
      cout<<"passed"<<endl;
    }
    else{
      cout<<"failed"<<endl;
      print_vectors(expected, actual);
    }

  }
  //check for memory leak
 if(!check_destruction){
    cout<<"memory leak detected"<<endl;
  }
 


 /*
  *Remove when the element is not in the set
  */
  {
    //create wordset
     cs3505::wordset myset(5);
     myset.add("aaaa"); //hashes to 0
     myset.add("bbbb"); //hashes to 0
     myset.add("cccc"); //hasehs to 0
     myset.add("1");    //hashes to 4
     myset.add("12");   //hashes to 3
     myset.add("123");  //hashes to 2
     
     //remove a non-existent element
     myset.remove("hello");
     
     //create a vector or expected elements
     vector<string> expected ;
     expected.push_back("aaaa");
     expected.push_back("bbbb");
     expected.push_back("cccc");
     expected.push_back("123");
     expected.push_back("12");
     expected.push_back("1");

     //get vector of elements in wordset
     vector<string> actual (myset.get_elements());

      cout<<"Test remove when element is not in the wordset:"<<endl;

      //print results 
      if(vectors_match(expected, actual)){
	cout<<"passed"<<endl;
      }
      else{
	cout<<"failed"<<endl;
	print_vectors(expected, actual);
      }

  }
  //check for memory leak
 if(!check_destruction){
    cout<<"memory leak detected"<<endl;
  }



 /*
  *Remove first element in linked list
  */
  {
    //create wordset
     cs3505::wordset myset(5);
     myset.add("aaaa"); //hashes to 0
     myset.add("bbbb"); //hashes to 0
     myset.add("cccc"); //hasehs to 0
     myset.add("1");    //hashes to 4    
     myset.add("12");   //hashes to 3
     myset.add("123");  //hashes to 2

     //remove a linked list header element
     myset.remove("aaaa");
     
     //creat vector of expected elements
     std::vector<std::string> expected;
     expected.push_back("bbbb");
     expected.push_back("cccc");
     expected.push_back("123");
     expected.push_back("12");
     expected.push_back("1");

     //get vector of elements in wordset
      std::vector<std::string> actual = myset.get_elements();
     cout<<"Test remove first element in linked list:"<<endl;

     //print results
     if(vectors_match(expected, actual)){
       cout<<"passed"<<endl;
     }
    else{
      cout<<"failed"<<endl;
      print_vectors(expected, actual);
    }
  }
  //check for memory leak
 if(!check_destruction){
    cout<<"memory leak detected"<<endl;
  }


 /*
  *Remove last element in linked list
  */
  {
    //create wordset
     cs3505::wordset myset(5);
     myset.add("aaaa"); //hashes to 0
     myset.add("bbbb"); //hashes to 0
     myset.add("cccc"); //hasehs to 0
     myset.add("1");    //hashes to 4
     myset.add("12");   //hashes to 3
     myset.add("123");  //hashes to 2

     //remove a linked list tail element
     myset.remove("cccc");

     //create a vector of expected elements 
     vector<string> expected ;
     expected.push_back("aaaa");
     expected.push_back("bbbb");
     expected.push_back("123");
     expected.push_back("12");
     expected.push_back("1");

     //get vector of wordset elements
     vector<string> actual (myset.get_elements());

     cout<<"Test remove last element in linked list:"<<endl;

     //print results 
    if(vectors_match(expected, actual)){
       cout<<"passed"<<endl;
     }
    else{
      cout<<"failed"<<endl;
      print_vectors(expected, actual);
    }
       
  }
  //check for memory leak
 if(!check_destruction){
    cout<<"memory leak detected"<<endl;
  }



 /*
  *Remove middle element in linked list
  */
  {
    //create wordset
     cs3505::wordset myset(5);
     myset.add("aaaa"); //hashes to 0
     myset.add("bbbb"); //hashes to 0
     myset.add("cccc"); //hasehs to 0
     myset.add("1");    //hashes to 4
     myset.add("12");   //hashes to 3
     myset.add("123");  //hashes to 2

     //remove middle linked list element
     myset.remove("bbbb");

     //create vector of expected elements 
     vector<string> expected ;
     expected.push_back("aaaa");
     expected.push_back("cccc");
     expected.push_back("123");
     expected.push_back("12");
     expected.push_back("1");

     //get vector of wordset elements
     vector<string> actual (myset.get_elements());

     //print results 
     cout<<"Test remove middle element of linked list:"<<endl;
     if(vectors_match(expected, actual)){
       cout<<"passed"<<endl;
     }
    else{
      cout<<"failed"<<endl;
      print_vectors(expected, actual);
    }
     
  }
  //check for memory leak
 if(!check_destruction){
    cout<<"memory leak detected"<<endl;
  }



 /*
  *contains true 
  */
  {
    //create wordset
     cs3505::wordset myset(5);
     myset.add("aaaa"); //hashes to 0
     myset.add("bbbb"); //hashes to 0
     myset.add("cccc"); //hasehs to 0
     myset.add("1");    //hashes to 4
     myset.add("12");   //hashes to 3
     myset.add("123");  //hashes to 2

     cout<<"Test contains"<<endl;

     //check that all elements are contained in wordset and print results 
     if(myset.contains("aaaa") && myset.contains("bbbb") && myset.contains("cccc") && myset.contains("1") && myset.contains("12") && myset.contains("123")){
       cout<<"passed"<<endl;
     }
     else{
       cout<<"failed: the wordset does not contain a string(s) that was added."<<endl;
     }
  }
  //check for memory leak
 if(!check_destruction){
    cout<<"memory leak detected"<<endl;
  }



 /* 
  *operator =
  */
  {
    //create first wordset
     cs3505::wordset first(5);
     first.add("aaaa"); //hashes to 0
     first.add("bbbb"); //hashes to 0
     first.add("cccc"); //hasehs to 0
     first.add("1");    //hashes to 4
     first.add("12");   //hashes to 3
     first.add("123");  //hashes to 2

     //create second wordset and set equal to first 
     cs3505::wordset second = first;

     //remove some elements from first wordset
     first.remove("aaaa");
     first.remove("123");

     cout<<"Test = "<<endl;

     //creat vector of expected elements 
     vector<string> expected ;
     expected.push_back("aaaa");
     expected.push_back("bbbb");
     expected.push_back("cccc");
     expected.push_back("123");
     expected.push_back("12");
     expected.push_back("1");

     //get vector of elements from second wordset
     vector<string> actual (second.get_elements());

     //print results 
     if(vectors_match(expected, actual)){
       cout<<"passed"<<endl;
     }
    else{
      cout<<"failed"<<endl;
      print_vectors(expected, actual);
    }
  }
  //check for memory leaks 
  if(!check_destruction){
    cout<<"memory leak detected"<<endl;
  }
  
  return 0;
}


/*
 * tester helper method 
 * takes in two vectors and returns true if they are the same length 
 * and if all of their elements are the same 
 */
bool vectors_match (vector<string> first, vector<string> second){
      // check that the sizes are equal
      if(first.size()!= second.size()){
	return false;
      }
      
      //travers the vectors
      for(int i = 0; i < first.size(); i++){

	//check that the elements are equal
	//if they are not, return false
	if(first[i] != second[i]){
	  return false; 
	}
      }
      //all of the elements are equal
      return true;
    }

/*
 * tester helper method
 * takes in two vectors and prints out their elements so that they can be compared
 * 
 */
void print_vectors(vector<string> expected, vector<string> actual){
      
    std:cout<<"Expected set elements:"<<std::endl;

      //traverse the first vector and print its elements with spaces in between 
      for(int i = 0; i< expected.size(); i ++){
	std::cout<<expected[i]<<"  ";
      }
      
      //new line
      std::cout<<" "<<endl;

      std::cout<<"Actual set elements:"<<std::endl;

      //traverse the second vector and print its elements 
      for(int i = 0 ; i < actual.size(); i++){
	std::cout<<actual[i]<<"  ";
      }

      //new line
      cout<<" "<<endl;
    }

/*
 * tester helper method
 * check that constructors and destructors are called the same number of times
 * returns true if they are, returns false if they are not
 */
bool check_destruction(){
      if(cs3505::wordset::constructor_count()== cs3505::wordset::destructor_count() 
	 && cs3505::node::constructor_count()== cs3505::node::destructor_count()){
	return true;
      }
      return false;
    }
