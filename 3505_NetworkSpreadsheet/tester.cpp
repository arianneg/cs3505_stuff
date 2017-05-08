#include <iostream>
#include "dependencygraph.h"
#include <list>
using namespace std;

int main(){


  DependencyGraph graph;

 
 graph.AddDependency("c1", "c2");
  graph.AddDependency("c2", "c3");
  // graph.AddDependency("a", "a");
  //
  // cout<<"size:"<<graph.GetDependees("a").size()<<endl;

  //  graph.RemoveDependency("a","a");

  //  cout<<"size:"<<graph.GetDependees("a").size()<<endl;



  //graph.AddDependency("c3", "c1");

 try{


  list<string> listRecalc = graph.GetCellsToRecalculate("c1");

  list<string>::iterator it;

  for(it = listRecalc.begin(); it!=listRecalc.end(); it++){
    cout<<*it <<endl;

  }

  cout<<listRecalc.size()<<endl;

 }catch(int param){

   cout<<param<<endl;
 }



  return 0;

}
 
