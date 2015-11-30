/////////////////////////////////////////////////////////////////////////////
// Server.h - socket server                                                //
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
*  This package is the socket server package
*
*  Required Files:
*  ---------------
*  Sockets.h, Sockets.cpp, AppHelpers.h, AppHelpers.cpp
*  Sender.h, Receiver.h
*
*  Members:
*  --------------
*  Server(int port, Socket::IpVer IpVer): server constructor
*  void StartListen(int port, Socket::IpVer IpVer): server strat listening to port
*  void StopListen(): server stop listen
*  void SendMsg(Msg msg):  send a message from the server
*  void eixt(Msg msg): server exit
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

#ifndef SERVER_H
#define SERVER_H

#include "Sockets.h"
#include "AppHelpers.h"
#include <iostream>
#include <thread>
#include <mutex>
#include "Sender.h"
#include "Receiver.h"

class Server
{
public:
	//get the current time by chrono::high_resolution_clock
	__int64 GetMSTime(){
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		__int64 time = std::chrono::duration_cast<std::chrono::milliseconds>(t1.time_since_epoch()).count();
		return time;
	}
	Server(int port, Socket::IpVer IpVer);//server constructor
	~Server(){};//destructor
	void StartListen(int port, Socket::IpVer IpVer);//server strat listening to port
	void StopListen();//server stop listen
	void SendMsg(Msg msg);// send a message from the server
	bool SendBin(Msg msg);
	void Server::GetFilelist(Msg &msg);
	void eixt(Msg msg);//server exit
	Msg RecvMsg(){ return rcvr.ReceiveMsg(); }
private:
	SocketSystem ss;
	Sender* sndr;
	Receiver rcvr;
};
#endif