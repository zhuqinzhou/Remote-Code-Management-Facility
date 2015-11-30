/////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - CSE687 Project #4, Spring 2015                        //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - MockChannel reads from sendQ and writes to recvQ                      //
// Qinzhou Zhu, CSE687 - Object Oriented Design, Spring 2015               //
// Original Author:  Jim Fawcett, Prof. CSE687                             //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  mock channel implementation for Pr4
*
*  Required Files:
*  ---------------
*  "MockChannel.h", "../Pr3/Cpp11-BlockingQueue.h" <string>
*  <thread>, <iostream>, <mutex>, "../Pr3/Sender.h"
*  "../Pr3/Receiver.h"
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 27 Apr 2015
*  - First Build
*/
#define IN_DLL
#include "MockChannel.h"
#include "../Pr3/Cpp11-BlockingQueue.h"
#include <string>
#include <thread>
#include <iostream>
#include <mutex>
#include "../Pr3/Sender.h"
#include "../Pr3/Receiver.h"

using BQueue = BlockingQueue < Message > ;

//client class
class client :public Iclient
{
public:
	//construct the client
	client(int port){
		sndr = new Sender();
		//std::cout << "\n  Create New Client Sender.";
		StartListen(port, "ip");
	};

	~client(){ /*delete sndr;*/ };//client desctructor

	//start client listening process
	void StartListen(int port, std::string IpVer){
		Socket::IpVer Ip = Socket::IP4;
		rcvr.StartListen(port, Ip, false);
	};

	//stop client listening process
	void StopListen(){
		Msg msg;
		msg.command("_EXIT");
		sndr->SendMsg(msg);
	}

	//send a message from client
	void SendMsg(Msg msg){ std::cout << "\n\n  Send Message: " + msg.command(); sndr->SendMsg(msg); };

	//send a file from client
	bool SendBin(Msg msg){ std::cout << "\n\n  Send Message: " + msg.command(); return sndr->SendBin(msg); };

	//client exit
	void eixt(Msg msg){ rcvr.eixt(msg); };

	//get a message form the receiver blockingQ
	Msg GetMsg() { return rcvr.ReceiveMsg(); };
private:
	SocketSystem ss;
	Sender* sndr;
	Receiver rcvr;
};

/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
	void postMessage(const Message& msg);
	BQueue& queue();
private:
	BQueue sendQ_;
};

void Sendr::postMessage(const Message& msg)
{
	sendQ_.enQ(msg);
}

BQueue& Sendr::queue() { return sendQ_; }

/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
	Message getMessage();
	BQueue& queue();
private:
	BQueue recvQ_;
};

Message Recvr::getMessage()
{
	return recvQ_.deQ();
}

BQueue& Recvr::queue()
{
	return recvQ_;
}
/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class MockChannel : public IMockChannel
{
public:
	MockChannel(ISendr* pSendr, IRecvr* pRecvr);
	void start();
	void stop();
private:
	std::thread thread_;
	ISendr* pISendr_;
	IRecvr* pIRecvr_;
	bool stop_ = false;
};

//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) {}


//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start()
{
	//std::cout << "\n  MockChannel starting up";
	thread_ = std::thread(
		[this] {
		Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
		Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
		if (pSendr == nullptr || pRecvr == nullptr)
		{
			std::cout << "\n  failed to start Mock Channel\n\n";
			return;
		}
		BQueue& sendQ = pSendr->queue();
		BQueue& recvQ = pRecvr->queue();
		while (!stop_)
		{
			//std::cout << "\n  channel deQing message";
			Message msg = sendQ.deQ();  // will block here so send quit message when stopping
			//std::cout << "\n  channel enQing message";
			recvQ.enQ(msg);
		}
		std::cout << "\n  Server stopping\n\n";
	});
}
//----< signal server thread to stop >---------------------------------------

void MockChannel::stop() { stop_ = true; }

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr)
{
	return new MockChannel(pISendr, pIRecvr);
}

Iclient* ObjectFactory::createclient(int port)
{
	return new client(port);
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
	ObjectFactory objFact;
	ISendr* pSendr = objFact.createSendr();
	IRecvr* pRecvr = objFact.createRecvr();
	IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
	pMockChannel->start();
	pSendr->postMessage("Hello World");
	pSendr->postMessage("CSE687 - Object Oriented Design");
	Message msg = pRecvr->getMessage();
	std::cout << "\n  received message = \"" << msg << "\"";
	msg = pRecvr->getMessage();
	std::cout << "\n  received message = \"" << msg << "\"";
	pSendr->postMessage("stopping");
	msg = pRecvr->getMessage();
	std::cout << "\n  received message = \"" << msg << "\"";
	pMockChannel->stop();
	pSendr->postMessage("quit");
	std::cin.get();
}
#endif
