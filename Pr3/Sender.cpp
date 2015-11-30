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
*  Sender.h, Cpp11-BlockingQueue.h, FileSystem.h
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
#include "Sockets.h"
#include "AppHelpers.h"
#include "Cpp11-BlockingQueue.h"
#include "Msg.h"
#include <thread>
#include "FileSystem.h"
#include "Sender.h"

BlockingQueue<Msg> Sender::SndrQ;

//create the sending thread
Sender::Sender()
{
	try{
		std::thread t1([&](){
			SocketSystem ss;
			while (true){
				Msg msg = SndrQ.deQ();
				if (msg.command() == "_EXIT"){
					std::cout << "\n  sndr stopped\n";
					break;
				}
				while (!si.connect(msg.destIP(), msg.destPort())){
					::Sleep(100);
				}
				si.sendString(msg.MsgtoString());
				//std::string ackmsg = si.recvString();
				//std::cout << "\n\n  Received Message: " + ackmsg;
			}
		});
		t1.detach();
	}
	catch (std::exception& excep){
		std::cout << excep.what() << std::endl;
		return;
	}
};

//desctructor
Sender::~Sender(){};

//send a message
void Sender::SendMsg(Msg msg){ SndrQ.enQ(msg); };

//send a file (any type)
bool Sender::SendBin(Msg msg)
{
	while (!si.connect(msg.destIP(), msg.destPort())){
		::Sleep(100);
	}
	FileSystem::File inTestFile(msg.fileName());
	inTestFile.open(FileSystem::File::in, FileSystem::File::binary);
	if (!inTestFile.isGood()){
		std::cout << "\n\n  Cannot open file: " + msg.fileName() << std::endl;
		return false;
	}
	std::cout << "\n\n  Uploading File: " + msg.fileName();
	Socket::byte buffer[1024];
	int PacLen;
	while (true)
	{
		PacLen = inTestFile.getBuffer(1024, buffer);
		msg.bodyLength(PacLen);
		si.sendString(msg.MsgtoString());
		std::string ackmsg = si.recvString();
		//std::cout << "\n" + ackmsg;
		si.send(PacLen, buffer);
		if (PacLen < 1024){
			inTestFile.close();
			break;
		}
	}
	return true;
};


#ifdef TEST_SENDER

int main()
{
	Client client(8080, Socket::IP4);
	term_count terminate;
	Msg msg;

	std::cout << "\n " << std::string(30, '=');
	demo.DemoReq3n6n7();
	msg.byname("server1");
	msg.SrcIP.second = "localhost"; msg.SrcPort.second = 8080; msg.Command = "Knock_From_1_client";
	client.SendMsg(msg);
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

#endif
