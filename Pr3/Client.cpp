/////////////////////////////////////////////////////////////////////////////
// Client.h - socket Client                                                //
// Ver 1.0                                                                 //
// Qinzhou Zhu, CSE#687 - Object O+riented Design, Spring 2015             //
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
*  This package is the socket Client package
*
*  Required Files:
*  ---------------
*  Client.h
*
*  Members:
*  --------------
*  Sender(): create the sending thread
*  void SendMsg(Msg msg): send a message
*  void SendBin(Msg msg): send a file (any type)
*  static BlockingQueue<Msg> SndrQ: sending queue
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
#include "../Pr3/sockets.h"
#include "../Pr3/AppHelpers.h"
#include <iostream>
#include <thread>
#include <mutex>
#include "../Pr3/Sender.h"
#include "../Pr3/Receiver.h"
#include "Client.h"

//contrucct the client
Client::Client(int port, Socket::IpVer IpVer){
	sndr = new Sender();
	StartListen(port, IpVer);
};

//start client listening process
void Client::StartListen(int port, Socket::IpVer IpVer){
	rcvr.StartListen(port, IpVer, false);
};

//stop client listening process
void Client::StopListen(){ rcvr.~Receiver(); delete sndr; }

//send a message from client
void Client::SendMsg(Msg msg){ std::cout << "\n\n  Send Message: " + msg.command(); sndr->SendMsg(msg); };

//send a file from client
void Client::SendBin(Msg msg){ std::cout << "\n\n  Send Message: " + msg.command(); sndr->SendBin(msg); };

//client exit
void Client::eixt(Msg msg){ rcvr.eixt(msg); };

#ifdef TEST_CLIENT

int main()
{
	Demo demo;
	HWND hwnd = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(hwnd, FALSE);
	DeleteMenu(hmenu, SC_CLOSE, MF_GRAYED);
	//client at localhost:8080
	title("Testing Client: 1_client", '=');
	Client client(8080, Socket::IP4);
	term_count terminate;
	Msg msg;

	msg.byname("server1");
	msg.SrcIP.second = "localhost"; msg.SrcPort.second = 8080; msg.Command = "Knock_From_1_client";
	client.SendMsg(msg);
	std::this_thread::sleep_for(std::chrono::seconds(1));

	msg.DestPort.second = 9081;
	msg.Command = "UpLoadFile";
	msg.FileName.second = "testsendbin.pdf";
	client.SendBin(msg);
}

#endif
