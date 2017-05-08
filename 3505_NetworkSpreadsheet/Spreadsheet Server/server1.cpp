#include <stdio.h>
#include <unistd.h> 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <mutex>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

using namespace std;
std::mutex mtx;

map<string,int> filename;
map<int, map<string,string> > spreadsheet;
map<int, string> clients; // Holds the list of clients currently connected to the server <int, string> -> <clientID, Username>
static int documentID;
static int clientID; // Represents the next available client ID
map<int,int> sockToClientID;
map<int,vector<int> > allOpenedSpreadsheet;


// Forward Declarations
void do_stuff(int sock);
string fileList();
void split (const string& s, char c,
	    vector<string>& v);
void cell_edit(int sock, vector<string> messageTokens);
void newSpreadsheet(int sock, vector<string> messageTokens);
void openSpreadsheet(int sock, vector<string> messageTokens);
void saveFile(int sock, vector<string> messageTokens);
void fileRename(int sock, vector<string> messageTokens);
void undo_edit(int sock, vector<string> messageTokens);
void redo_edit(int sock, vector<string> messageTokens);
void showFileList(int sock, vector<string> messageTokens);
void closeSpreadsheet (int sock, vector<string> messageTokens);

int main(int argc, char** argv)
{
  int sockfd, newsockfd, pid;
  int serverPort = 2112;
  clientID = -1;
  struct sockaddr_in server_addr;
  socklen_t size;

  // Create a new TCP socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
    {
      cout << "\nError establishing the connection!" << endl;
      exit(1);
    }

  // The the values in a buffer to zero
  bzero((char *) &server_addr, sizeof(server_addr));

  server_addr.sin_family = AF_INET;                // The family of addresses accepted (IPv4 and IPv6)
  server_addr.sin_addr.s_addr = htons(INADDR_ANY); // Holds the IP address used for the socket (The Server's IP)
  server_addr.sin_port = htons(serverPort);        // Port number for the socket to use

  // Bind the socket to the IP address and port number
  if ((bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0)
    {
      cout << "\nError binding connection!\n" << endl;
      return -1;
    }

  cout << "\nServer started successfully...\n" << endl;

  size = sizeof(server_addr);
  cout << "Listening for clients..." << endl;

  // Listen for socket connections
  listen(sockfd, 5);

  while (1)
    {
      // Accept a new client
      newsockfd = accept(sockfd, (struct sockaddr *)&server_addr, &size);
	  cout << "newsockfd: " << newsockfd << endl;

      if (newsockfd < 0)
	cout << "\nError accepting client!\n" << endl;

      // Create a new thread for this client
      pid = fork();

      if (pid < 0)
	cout << "\nError on fork!\n" << endl;

      if (pid == 0)
	  {
	    //mtx.lock();
	    close(sockfd);
	    clientID += 1;
	    string clientIDStr;
	    ostringstream convert;
	    convert << clientID;
	    clientIDStr = convert.str() + '\n';
	    sockToClientID[clientID] = newsockfd;
	    for (map<int,int>::iterator it = sockToClientID.begin(); it != sockToClientID.end(); ++it)
	    {
	      cout << "Client ID: " << it->first << " Client Sock: " << it->second << endl; 
	    }
	    write(newsockfd, clientIDStr.c_str(), 32);
	    do_stuff(newsockfd);
	    exit(0);
	    //mtx.unlock();
	}
      else
	close(newsockfd);
    }

  close(sockfd);
  return 0;
  
}

void split(const string& s, char c, vector<string>& v) 
{
  string::size_type i = 0;
  string::size_type j = s.find(c);

  while (j != string::npos) {
    v.push_back(s.substr(i, j-i));
    i = ++j;
    j = s.find(c, j);

    if (j == string::npos)
      v.push_back(s.substr(i, s.length()));
  }
}


void do_stuff(int sock)
{
  int n;
  char buffer[1024];
  vector<string> messages;
  vector<string> messageTokens;
  bool isUserName = true;

  while (1)
    {
      bzero(buffer, 1024);
      n = read(sock, buffer, 1024);
      if (n < 0)
	cout << "\nError on reading from socket!\n" << endl;

      // Convert the entire buffer into a single string
      string incomingData(buffer);

      if (incomingData.find('\n') != string::npos)
	{
	  printf("Entire message from client: %s\n", buffer);

	  char *tok;
	  tok = strtok(buffer, " \n"); // Split on tab characters and a newline character

	  // Push all tokens from a single message to a vector
	  while (tok != NULL)
	    {
	      messages.push_back(tok);
	      tok = strtok(NULL, " \n");
	    }

	  for (int i = 0; i < messages.size(); i++)
	    {
	      cout << "Message: " << messages[i] << endl;

	      istringstream iss(messages[i]);
	      string token;
	      while (getline(iss, token, '\t'))
		{
		  messageTokens.push_back(token);
		}
	    }

	  for (int i = 0; i < messageTokens.size(); i++)
	    {
	      cout << "Message Token: " << messageTokens[i] << endl;
	    }

	  // Get the message type (opcode)
	  int opCode = atoi(messageTokens[0].c_str());

	  // The first thing received from the client should always be the username
	  if (isUserName)
	    {
	      opCode = -1;
	      isUserName = false;
	      clients.insert(pair<int, string>(clientID, messageTokens[0]));	
	      sockToClientID[sock]=clientID;
	    }

	  cout << "OpCode: " << opCode << endl;

	  // Call the appropriate functions based on the opCode
	  switch(opCode)
	    {
	    case 0:
	      showFileList(sock,messageTokens);
	      break;
	    case 1:
	      //n = write(sock, (newFile(incomingData)).c_str(), 1024);
	      newSpreadsheet(sock, messageTokens);
	      break;
	    case 2:
	      //n = write(sock, (openFile(incomingData)).c_str(), 1024);
	      openSpreadsheet(sock, messageTokens);
	      break;
	    case 3:
	      cell_edit(sock, messageTokens);
	      break;
	    case 4:
	      undo_edit(sock, messageTokens);
	      break;
	    case 5:
	      redo_edit(sock, messageTokens);
	      break;
	    case 6:
	      saveFile(sock, messageTokens);
	      break;
	    case 7:
	      fileRename(sock, messageTokens);
	      break;
	    case 9:
	      //closeSpreadsheet (sock, messageTokens);
	      break;
	    default:
	      // Do Nothing
	      break;
	    }
	}

      messages.clear();
      messageTokens.clear();
    }
}

// Check if file name is not existed. Then assign new DocID for this file
// if the file exists, send a list of available spreadsheet
void newSpreadsheet(int sock, vector<string>messageTokens){
  int n;
  string name = messageTokens[1];
  if (filename.find(name)==filename.end()){
    filename[name]=documentID;
    n = write(sock,("1\t"+std::to_string(documentID)+"\n").c_str() ,1024);
    documentID++;   
  }
  else{
    showFileList(sock, messageTokens);
  }
}

void openSpreadsheet(int sock, vector<string>messageTokens){
  int n;
  string name = messageTokens[1];
  map<string,int>::iterator d = filename.find(name);
  
  if (d !=filename.end()){
    int DocID = d->second;
    map<int,map<string,string>>::iterator s = spreadsheet.find(DocID);

    if(s != spreadsheet.end())
      {
	map<string,string> currentSpreadsheet = s->second;
	for(map<string,string>::iterator it=currentSpreadsheet.begin();it!=currentSpreadsheet.end();it++){
	  string cellName = it->first;
	  string cellContent = it->second;
	  n = write(sock, ("3\t"+std::to_string(DocID)+"\t" + cellName + "\t" + cellContent + "\n").c_str(), 1024);
	}
      }
  }
  else{
    showFileList(sock, messageTokens);
  }
}

// Function that handles cell edits
void cell_edit(int sock, vector<string> messageTokens)
{
  int n;
  int DocID = atoi(messageTokens[1].c_str());
  string cellName = messageTokens[2];
  string newContents = messageTokens[3];

  // Circular Dependency Check Still Required

  // Save Cell Contents for Undo Still Required

  // Save cell name and cell contents
  spreadsheet[DocID][cellName] = newContents;
  n = write(sock, ("4\t" + messageTokens[1]+"\n").c_str(), 32); // Sends a valid message
  n = write(sock, ("3\t" + messageTokens[1]+"\t" + cellName + "\t" + newContents + "\n").c_str(), 1024); // Send the cell edit
}

void saveFile(int sock, vector<string>messageTokens){
  int n;
  string name = messageTokens[1];
  map<string,int>::iterator d = filename.find(name);
  if (d !=filename.end()){
    int DocID = d->second;
    n = write(sock,("7\t"+std::to_string(DocID)+"\n").c_str() ,1024);
  }
}

void fileRename(int sock, vector<string>messageTokens){
  int n;
  int DocID = atoi(messageTokens[1].c_str());
  string newFileName = messageTokens[2];

  
  if (filename.find(newFileName)==filename.end()){
    for(map<string,int>::iterator it = filename.begin();it!=filename.end();it++){
      if(it->second == DocID){
	filename.erase(it);
	filename[newFileName] = DocID;
	n = write(sock, ("6\t"+std::to_string(DocID)+"\t" +newFileName+"\n").c_str(), 1024);
      }
    }
  }
  else{
    n = write(sock, ("9\t"+std::to_string(DocID)+"\n").c_str(), 1024);
  }
}

void undo_edit(int sock, vector<string> messageTokens)
{
  cout << "UNDO CALLED!" << endl;
}

void redo_edit(int sock, vector<string> messageTokens)
{
  cout << "REDO CALLED!" << endl;
}

void showFileList(int sock, vector<string> messageTokens){
  string fileNames="0\t";
  for (map<string,int>::iterator it = filename.begin(); it != filename.end(); ++it)
    {
      string temp = it->first + "\t";
      fileNames.append(temp);
    }
  fileNames.append("\n");
  int n = write(sock,fileNames.c_str(), 1024);
}

/*void closeSpreadsheet (int sock, vector<string> messageTokens)
{
  int id = sockToClientID[sock];
  int doc_ID=messageTokens[1];
  vector<int> socks=allOpenedSpreadsheet[doc_ID];
  
  }*/
