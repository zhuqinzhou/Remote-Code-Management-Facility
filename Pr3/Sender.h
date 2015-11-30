/////////////////////////////////////////////////////////////////////////////
// Sender.h - socket sender                                                //
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
*  This package is the sender package
*
*  Required Files:
*  ---------------
*  ClientTest.cpp
*  Sockets.h, Sockets.cpp, AppHelpers.h, AppHelpers.cpp
*  Cpp11-BlockingQueue.h, FileSystem.h
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
#ifndef SENDER_H
#define SENDER_H
#include "Sockets.h"
#include "AppHelpers.h"
#include "Cpp11-BlockingQueue.h"
#include "Msg.h"
#include <thread>
#include "FileSystem.h"

class Sender
{
public:
	Sender();//create the sending thread
	~Sender();//desctructor
	void SendMsg(Msg msg);//send a message
	bool SendBin(Msg msg);//send a file (any type)
private:
	static BlockingQueue<Msg> SndrQ;//sending queue
	SocketConnecter si;
};

#endif;