#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
/////////////////////////////////////////////////////////////////////////////
// MockChannel.h - Demo for CSE687 Project #4, Spring 2015                 //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - MockChannel reads from sendQ and writes to recvQ                      //
//                                                                         //
// Qinzhou Zhu, CSE687 - Object Oriented Design, Spring 2015               //
// Original Author:  Jim Fawcett, Prof. CSE687                             //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  mock channel header for Pr4. Iclient interface was added
*  inorder to use the client functions build in Pr3 on GUI
*
*  Required Files:
*  ---------------
*  <string>, "../Pr3/Msg.h"
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
#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
#include "../Pr3/Msg.h"
using Message = std::string;

//client interface
struct Iclient
{
	virtual void StartListen(int port, std::string IpVer) = 0;//start client listening process
	virtual void StopListen() = 0;//stop client listening process
	virtual void SendMsg(Msg msg) = 0;//send a message from client
	virtual bool SendBin(Msg msg) = 0;//send a file from client
	virtual void eixt(Msg msg) = 0;//client exit
	virtual Msg GetMsg() = 0;// get a message from the receiver
};

//mockchannel sender interface
struct ISendr
{
	virtual void postMessage(const Message& msg) = 0;
};

//mockchannel receiver interface
struct IRecvr
{
	virtual std::string getMessage() = 0;
};

struct IMockChannel
{
public:
	virtual void start() = 0;
	virtual void stop() = 0;
};

extern "C" {
	struct ObjectFactory
	{
		DLL_DECL ISendr* createSendr();
		DLL_DECL IRecvr* createRecvr();
		DLL_DECL IMockChannel* createMockChannel(ISendr* pISendr, IRecvr* pIRecvr);
		DLL_DECL Iclient* createclient(int port);
	};
}

#endif


