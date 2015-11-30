/////////////////////////////////////////////////////////////////////////////
// Receiver.h- receiver                                                    //
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
*  This package provide the receiver functions
*
*  Required Files:
*  ---------------
*  Sockets.h
*  Cpp11-BlockingQueue.h
*  Msg.h
*  AppHelpers.h
*  FileSystem.h
*
*  Members:
*  --------------
*  void operator()(Socket& socket_): handler for receiver
*  void StopListen(): stop receiver listening (helper for receiver.stoplisten)
*  Msg DeQ(): deq from receiving queue
*  bool ReceiveBin(Msg header, std::string filename, Socket& socket_): receive a file from sender
*
*  void StartListen(int port, Socket::IpVer IpVer): starting receiver listening
*  void StopListen(): stop receiver listening
*  Msg ReceiveMsg(): receive a message from sender
*  void eixt(Msg msg): enq the exit message
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
#ifndef RECEIVER_H
#define RECEIVER_H
#include "Sockets.h"
#include "Cpp11-BlockingQueue.h"
#include "Msg.h"
#include "AppHelpers.h"
#include "FileSystem.h"
#include<thread>
#include<fstream>

class ClientHandler
{
public:
	void operator()(Socket& socket_);//handler for receiver
	Msg DeQ();//deq from receiving queue
	void cpeixt(Msg msg){ RecvQ.enQ(msg); };//enq the exit message
	bool ReceiveBin(Msg header, std::string filename, Socket& socket_);//receive a file from sender
private:
	static BlockingQueue<Msg> RecvQ;//receiving queue
};

class Receiver
{
public:
	Receiver(){};//constructor
	~Receiver(){};//desctructor
	void StartListen(int port, Socket::IpVer IpVer,bool server);//starting receiver listening
	void StopListen();//stop receiver listening
	Msg ReceiveMsg();//receive a message from sender
	void eixt(Msg msg);//enq the exit message
private:
	SocketListener *sl;
	ClientHandler cp;
	BlockingQueue<std::string> *channel;//receiving queue
};


#endif