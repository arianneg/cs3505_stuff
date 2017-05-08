#include "dependencygraph.h"
#include <iostream>
using namespace std;

//constructor 
DependencyGraph::DependencyGraph(){

  size = 0;

}

//returns number of dependencies within graph 
int DependencyGraph::Size(){


  return size;
}

//returns the number of dependees of the given string 
int DependencyGraph::getNumberOfDependees(string s ){
    return this -> GetDependees(s).size();

}



//returns true if the given string has dependents
bool DependencyGraph::HasDependents(string s){

  //look for s 
  if (sorted.find(s) != sorted.end()){
    //if s has dependents, return true 
    if(sorted.find(s)->second.getStringDependentsList().size()>0){
      return true;
    }
    
    else{
      return false;

    }
  }
  return false;
}


//returns true if the given string has dependents
bool DependencyGraph::HasDependees(string s){
  //look for s 
  if(sorted.find(s)!= sorted.end()){
    //if s has dependees return true 
    if(sorted.find(s)-> second.getStringDependeesList().size()>0){
      return true;

    }
    else{
      return false;
    }

  }
  return false;
}

//returns a set containing the dependents of s 
  set<string> DependencyGraph::GetDependents(string s){
    //look for s in the graph 
    if(sorted.find(s)!= sorted.end() ){
      //call node function to get dependents 
      return sorted.find(s)->second.getStringDependentsList();

    }
    else{
      //if s is not in the graph, return empty set
      set<string> empty;
      return empty;}
    
  }
//returns a set containing the dependees of s 
    set<string> DependencyGraph::GetDependees(string s){

      //look for s in graph 
      if(sorted.find(s)!= sorted.end()){
	//call node function to get dependents 
	return sorted.find(s)-> second.getStringDependeesList();
      }
      //if s is not in the graph, return empty set 
      else{
	set<string>empty;
	return empty;}

    }
//adds a dependency, where s is the dependent and t is the dependee 
void  DependencyGraph::AddDependency(string s , string t){
  
  //if both s and t are already in the graph 
  if(sorted.find(s)!=sorted.end() && sorted.find(t)!=sorted.end()){

    //get the nodes for s and t 
    Node sNode = sorted.find(s)->second;
    Node tNode = sorted.find(t)->second;
  
    //get the lists of dependents/dependees for s and t 
    set<string> sSet = sNode.getStringDependentsList();
    set<string> tSet = tNode.getStringDependeesList();

    //if the given dependency already exists, do nothing 
    if(sSet.find(t)!= sSet.end() &&tSet.find(t)!= tSet.end()){
      
      return;
    }
   
    //add s to t's list of dependents 
    //add t to s's list of dependees 
    else{
      sorted.find(s)->second.addDependents(t);
      sorted.find(t)->second.addDependees(s);
      size++;
     

    }
  }
  //If neither s nor t were already in the graph 
  else if(sorted.find(s) == sorted.end() && sorted.find(t) == sorted.end()){
 
    //make nodes 
    Node sNodeNew ;
    Node tNodeNew;

    //set s into corresponding node 
    sNodeNew.setData(s);
    
    //if s and t are equal, then only need to add s to itself as dependent and dependee
    if(s == t){
     
      sNodeNew.addDependents(s);
      sNodeNew.addDependees(s);
      pair<string, Node> Spaired;
      Spaired = make_pair(s,sNodeNew);
      sorted.insert(Spaired);
      size++;
      return;

    }

    //if s and t are not equal, add dependency 
    sNodeNew.addDependents(t);
    pair<string, Node> Spaired;
    Spaired = make_pair(s,sNodeNew);
    sorted.insert(Spaired);

    //set t in corresponding node 
    tNodeNew.setData(t);
    set<string> tSet = tNodeNew.getStringDependeesList();
    
    //add dependency 
    tNodeNew.addDependees(s);
    pair<string, Node> Tpaired;
    Tpaired = make_pair(t,tNodeNew);
    sorted.insert(Tpaired);

    set<string> tSet2 = tNodeNew.getStringDependeesList();
   
    size++;
   
    return;
  }//end of new s and t 

  //if s was in the graph but t was not 
  else if(sorted.find(s) != sorted.end() && sorted.find(t) == sorted.end()){
     
    sorted.find(s)->second.addDependents(t);
    Node tNodeNew;
    tNodeNew.setData(t);
    tNodeNew.addDependees(s);
    pair<string, Node> Tpaired;
    Tpaired = make_pair(t, tNodeNew);
    sorted.insert(Tpaired);
    size++;
    return;
  }

  //if t was in the graph but s was not 
  else if(sorted.find(t)!= sorted.end() && sorted.find(s) == sorted.end()){
 
    sorted.find(t)->second.addDependees(s);
    Node sNodeNew;
    sNodeNew.setData(s);
    sNodeNew.addDependents(t);
    pair<string, Node> Spaired;
    Spaired = make_pair(s, sNodeNew);
    sorted.insert(Spaired);
    size++;
    return;

  }
  
}


//remove dependency the dependency where s is the dependent and t is the dependee 
void DependencyGraph::RemoveDependency(string s, string t){

  
  //if s and t are in the graph 
  if(sorted.find(s) != sorted.end() && sorted.find(t) != sorted.end()){
    //remove t from s 
    sorted.find(s)->second.removeDependents(t);

    //if s now has no dependents or dependees, remove s from graph 
    if(sorted.find(s)->second.getStringDependentsList().size() == 0 && sorted.find(s)->second.getStringDependeesList().size() == 0){
      sorted.erase(s);
      

    }
    //remove s from t 
    sorted.find(t)->second.removeDependees(s);
    //t now has no dependents or dependees, remove t from graph 
    if(sorted.find(t)->second.getStringDependentsList().size() ==0 && sorted.find(t)->second.getStringDependeesList().size()== 0 ){

      sorted.erase(t);

    }
    size--;


  }
 }




//returns a set of immediate dependents of the given string  
set<string> DependencyGraph::GetDirectDependents(string t){

  return GetDependents(t);

}

//recursive method to traverse all dependents of a given member of the graph and store all dependents 
//throws exception if circular dependency is detected 
void DependencyGraph::Visit(string start, string name, set<string> &visited, list<string> &changed){
  
  //mark current member as visited 
  visited.insert(name);

  //get immediate dependents 
  set<string> directDependents = GetDirectDependents(name);

  set<string>::iterator it;
  
  //for each dependents of the current member 
  for(it = directDependents.begin(); it!= directDependents.end(); it++){
    
    //thow if circulare dependency is detected 
    if(*it == start){
      
      throw 0;

    }
    //recursive call 
    else if(visited.find(*it) == visited.end()){
      
      Visit(start, *it, visited, changed);

    }

  }
  //store dependent 
  changed.push_front(name);
  
  

}

//set up method for recursive method visit
//return a list of all dependents for all of the graph members in the given set 
list<string> DependencyGraph::GetCellsToRecalculate(set<string> names){

  //set up data structures for storage 
  list<string> changed;
  set<string> visited;
  
  
  set<string>::iterator it;

  //loop through set 
  for(it = names.begin(); it != names.end(); it++){
    if(visited.find(*it) == visited.end()){
      
      //call recursive method 
      Visit(*it, *it, visited, changed);

    }

  }
  return changed;

}

//allows GetCellsToRecalculate(set<string>) to be called with only one string 
list<string> DependencyGraph::GetCellsToRecalculate(string name){
  
 set<string> names;
  names.insert(name);
  return GetCellsToRecalculate(names);
  
}
  



