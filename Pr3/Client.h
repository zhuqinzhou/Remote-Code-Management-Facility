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
*  Sockets.h, Sockets.cpp, AppHelpers.h, AppHelpers.cpp
*  Sender.h, Client.h, Receiver.h
*
*  Members:
*  --------------
*  Client(int port, Socket::IpVer IpVer): contrucct the client
*  void StartListen(int port, Socket::IpVer IpVer): start client listening process
*  void StopListen(): stop client listening process
*  void SendMsg(Msg msg): send a message from client
*  void SendBin(Msg msg): send a file from client
*  void eixt(Msg msg): client exit
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
#ifndef CLIENT_H
#define CLIENT_H

#include "Sockets.h"
#include "AppHelpers.h"
#include <iostream>
#include <thread>
#include <mutex>
#include "Sender.h"
#include "Receiver.h"

class Client
{
public:
	Client(int port, Socket::IpVer IpVer);//contrucct the client
	~Client(){};//client desctructor
	void StartListen(int port, Socket::IpVer IpVer);//start client listening process
	void StopListen();//stop client listening process
	void SendMsg(Msg msg);//send a message from client
	void SendBin(Msg msg);//send a file from client
	void eixt(Msg msg);//client exit
private:
	SocketSystem ss;
	Sender* sndr;
	Receiver rcvr;
};

#endif