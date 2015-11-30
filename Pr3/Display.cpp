/////////////////////////////////////////////////////////////////////////////
// Display.cpp - display package                                           //
// Ver 1.0                                                                 //
// Qinzhou Zhu, CSE#687 - Object Oriented Design, Spring 2015              //
//          Syracuse University, qzhu100@syr.edu                           //
//-------------------------------------------------------------------------//
// Qinzhou Zhu (c) copyright 2015                                          //
// All rights granted provided this copyright notice is retained           //
//-------------------------------------------------------------------------//
// Application: Demo for Project #3, Communication Channel                 //
// Platform:    Win 7 x64, Visual Studio 2013                              //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package is the test executive package for server1 at localhost:9080
*
*  Required Files:
*  ---------------
*  Display.h
*
*  Build Command:
*  --------------
*  devenv Sockets.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 12 Apr 15
*  - first release
*/
#include <iostream>
#include "Display.h"

using namespace std;

//demo requirement3,6,7
void Demo::DemoReq3n6n7()
{
	cout << "\n  -Demo-requirement-3-:\n  use Sockets to implement a Message-Passing Communication facility that transports messages to an endpoint specified by an ip address and port number or network name.";
	cout << "\n  In client1, message was sent by telling the name of server (\"server1\") to server1";
	cout << "\n  In client2, message was sent by setting the ip (localhost) and port number (9080) of server1.";
	cout << "\n  -Demo-requirement-7-:\n  demonstrate the construction of a peer-to-peer communication channel that has a sender and receiver on each end and the sending of a request and receipt of a reply on one end and the construction of the reply on the other end";
	cout << "\n  In the following demo, a \"knock\" message would be sent to server1 and server2 by each client";
	cout << "\n  After the server received it, it would reply an \"ACK\" message to the client";
	cout << "\n  Thus, there're senders and receivers on each end.";
	cout << "\n  In the demo of requirement5 (sending files), clients would sending files to a different server (server2)";
	cout << "\n  Thus, it is feasible to communicate with a specified endpoint.";
}

//demo requirement4
void Demo::DemoReq4(){
	cout << "\n  -Demo-requirement-4-:\n  Provide a class for constructing and interpreting messages.";
	cout << "\n  The Message (Msg.h) classes is located in path: .\\Message";
};

//demo requirement5
void Demo::DemoReq5(){
	cout << "\n  -Demo-requirement-5-:\n  Support transporting binary and text files to a specified endpoint.";
	cout << "\n  After sending the file, you shall see the text (testtxtsend.txt) and binary file (testsendbin.pdf) in path .\\2_Server";
	cout << "\n  The files were sent from the client1 and client2 directories";
};

//demo requirement6,8
void Demo::DemoReq6n8(){
	cout << "\n  -Demo-requirement-6-:\n  provide sender and receiver classes that encapsulate the communication channel facilities into easy to use packages.";
	cout << "\n  The sender (Sender.h) and receiver (Receiver.h) classes are located in path: .\\SndrRcvr";
	cout << "\n  Methods like send/receive a message were defined in them, whilch encapsulated the communication channel facilities";
	cout << "\n  In addition, the client (client.h) and server (server.h) classes, were used to defined the properties of client and server.";
	cout << "\n  They furtherly encapsulate the methods in sender and receiver class.";
	cout << "\n  Please look into those files for details. ";
	cout << "\n  -Demo-requirement-8-:\n  Please look into display.h for display package; Exec_client#.cpp and Exec_server#.cpp for test executives.";
};

#ifdef TEST_DISPLAY

int main()
{
	DemoReq5();
	DemoReq6n8();
}

#endif
