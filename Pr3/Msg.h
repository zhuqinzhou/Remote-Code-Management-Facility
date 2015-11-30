/////////////////////////////////////////////////////////////////////////////
// Msg.h - socket sender                                                //
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
*  This package is the msg constructing and interpreting package
*
*  Required Files:
*  ---------------
*  none
*
*  Members:
*  --------------
* void byname(std::string name): set the endpoint information by server name
* void construct_message(std::string srcip, int srcport, std::string destip, int destport, std::string filename, std::string body, int bodylength): constructing message
* std::string MsgtoString(): transform message to string
* void StringtoMsg(std::string str): interpreting string and contruct message
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

#ifndef MSG_H
#define MSG_H
#include <iostream>
#include<vector>
#include<string>
class Msg
{
public:
	Msg();
	~Msg(){};
	void byname(std::string name);//set the endpoint information by server name
	void construct_message(std::string srcip, int srcport, std::string destip, int destport, std::string filename, std::string body, int bodylength);//constructing message
	std::string MsgtoString();//transform message to string
	void StringtoMsg(std::string str);//interpreting string and contruct message
	void StrToMsghelper(std::string &name, std::string &value);
	void reverse(){
		std::string tempip = SrcIP.second;
		int tempport = SrcPort.second;
		SrcIP.second = DestIP.second;
		SrcPort.second = DestPort.second;
		DestIP.second = tempip;
		DestPort.second = tempport;
	}
	//the following functions are private members returning and modification methods
	std::string command(){ return Command; };
	void command(std::string c){ Command = c; };
	std::string srcIP(){ return SrcIP.second; };
	void srcIP(std::string si){ SrcIP.second = si; };
	int srcPort(){ return SrcPort.second; };
	void srcPort(int sp){ SrcPort.second = sp; };
	std::string destIP(){ return DestIP.second; };
	void destIP(std::string di){ DestIP.second = di; };
	int destPort(){ return DestPort.second; };
	void destPort(int dp){ DestPort.second = dp; };
	std::string fileName(){ return FileName.second; }
	void fileName(std::string fn){ FileName.second = fn; }
	std::string body(){ return Body.second; }
	void body(std::string b){ Body.second = b; }
	int bodyLength(){ return BodyLength.second; }
	void bodyLength(int bl){ BodyLength.second = bl; }
	__int64 time(){ return Time.second; };
	void time(__int64 t){ Time.second = t; }
private:
	std::string Command;
	std::pair<std::string, std::string> SrcIP;
	std::pair<std::string, int> SrcPort;
	std::pair<std::string, std::string> DestIP;
	std::pair<std::string, int> DestPort;
	std::pair<std::string, std::string> FileName;
	std::pair<std::string, std::string> Body;
	std::pair<std::string, int> BodyLength;
	std::pair<std::string, __int64> Time;
};
#endif