/////////////////////////////////////////////////////////////////////////////
// Receiver.cpp- receiver implementation                                   //
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
*  Receiver.h
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

#include "Receiver.h"
#include <chrono>
using namespace ApplicationHelpers;

//get the current time
__int64 GetMSTime()
{
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	__int64 time = std::chrono::duration_cast<std::chrono::milliseconds>(t1.time_since_epoch()).count();
	return time;
}

//get the file name from a string
std::string getfilename(std::string fullname){
	int ind1 = fullname.find_last_of('/');
	int ind2 = fullname.find_last_of('\\');
	if (ind1 > ind2)
		return fullname.substr(ind1 + 1);
	else
		return fullname.substr(ind2 + 1);
}

//receiving queue
BlockingQueue<Msg> ClientHandler::RecvQ;

//handler for receiver
void ClientHandler::operator()(Socket& socket_)
{
	
	Msg msg;
	// interpret test command
	std::string str = socket_.recvString();
	msg.StringtoMsg(str);
	if (msg.command() == "UpLoadFile"){
		__int64 t1 = GetMSTime();
		bool success = ReceiveBin(msg, getfilename(msg.fileName()), socket_);
		Msg msg;
		msg.StringtoMsg(str);
		if (success)
			msg.command("UpLoadSuccess");
		else
			msg.command("UpLoadFailed");

		__int64 t2 = GetMSTime();
		std::cout << "\n  Processing Time(millisecond) = " + std::to_string(t2 - t1);
		RecvQ.enQ(msg);
	}
	else if (msg.command() == "UpLoadSuccess" || msg.command() == "UpLoadFailed"){
		std::cout << "\n  File " + msg.command();
		RecvQ.enQ(msg);
	}
	else if (msg.command() == "DownLoadFile"){
		RecvQ.enQ(msg);
	}
	else if (msg.command() == "GetFiles"){
		this->RecvQ.enQ(msg);
	}
	else if (msg.command() == "GetFilesResult"){ RecvQ.enQ(msg); }
	else{
		socket_.sendString("Server_ACK for \"" + msg.command() + '\"');
		Msg msg; msg.StringtoMsg(str);
		RecvQ.enQ(msg);
	}
	// we get here if command isn't requesting a test
	//Verbose::show("ClientHandler socket connection closing");
	socket_.shutDown();
	socket_.close();
	//Verbose::show("ClientHandler thread terminating");
};

//deq from receiving queue
Msg ClientHandler::DeQ(){
	return RecvQ.deQ();
}

//receive a file from sender
bool ClientHandler::ReceiveBin(Msg header, std::string filename, Socket& socket_)
{
	if (header.destPort() == 9080)
		filename = "../1_Server/" + filename;
	if (header.destPort() == 9081)
		filename = "../2_Server/" + filename;
	FileSystem::File file(filename);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood()){
		std::cout << "\n  Bad file: " + filename + " \n";
		return false;
	}
	Socket::byte buffer[1024];
	int counter = 0;
	while (true){
		socket_.sendString("ack" + std::to_string(counter));
		counter++;
		if (counter > 1)
		{
			std::string header_str = socket_.recvString();
			header.StringtoMsg(header_str);
		}
		bool b = socket_.recv(header.bodyLength(), buffer);
		file.putBuffer(header.bodyLength(), buffer);
		if (header.bodyLength() < 1024)
			break;
	}
	file.close();
	std::cout << "\n  File receive successfully: " + filename;
	return true;
}

//starting receiver listening
void Receiver::StartListen(int port, Socket::IpVer IpVer, bool server){
	sl = new SocketListener(port, IpVer);
	(*sl).start(cp);
};

//stop receiver listening
void Receiver::StopListen(){ delete sl; };

//receive a message from sender
Msg Receiver::ReceiveMsg(){
	return cp.DeQ();
}

//enq the exit message
void Receiver::eixt(Msg msg){ cp.cpeixt(msg); };

#ifdef TEST_RECEIVER

int main()
{
	Server server(9080, Socket::IpVer::IP4);
	term_count terminate;
	std::this_thread::sleep_for(std::chrono::seconds(3));
	demo.DemoReq6n8();
	std::cout << "\n  press key to exit: ";
	signal(SIGINT, catch_int);
	do{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		if (terminate.c_count() == 1){
			server.StopListen();
			return 0;
		}
	} while (!std::cin.get());
	server.StopListen();
}

#endif
