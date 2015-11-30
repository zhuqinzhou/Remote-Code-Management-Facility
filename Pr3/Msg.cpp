/////////////////////////////////////////////////////////////////////////////
// Msg.h - socket sender                                                   //
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
*  Msg.h
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
#include "Msg.h"
#include <iostream>
#include<vector>
#include<string>

//message constructor
Msg::Msg(){
	SrcIP.first = "SrcIP"; SrcPort.first = "SrcPort"; DestIP.first = "DestIP"; DestPort.first = "DestPort";
	FileName.first = "FileName"; Body.first = "Body"; BodyLength.first = "BodyLength"; Time.first = "Time_Millisecond";
};

// set the endpoint information by server name
void Msg::byname(std::string name)
{
	if (name == "server1"){
		DestIP.second = "127.0.0.1"; DestPort.second = 9080;
	}
	if (name == "server2"){
		DestIP.second = "127.0.0.1"; DestPort.second = 9081;
	}
}

//constructing message
void Msg::construct_message(std::string srcip, int srcport, std::string destip, int destport, std::string filename, std::string body, int bodylength)
{
	SrcIP.second = srcip; SrcPort.second = srcport; DestIP.second = destip; DestPort.second = destport;
	FileName.second = filename; Body.second = body; BodyLength.second = bodylength;
}

//transform message to string
std::string Msg::MsgtoString(){
	return Command + "\n" + SrcIP.first + ":" + SrcIP.second + "\n" + SrcPort.first + ":" + std::to_string(SrcPort.second) + "\n"
		+ DestIP.first + ":" + DestIP.second + "\n" + DestPort.first + ":" + std::to_string(DestPort.second) + "\n" + FileName.first + ":" + FileName.second + "\n"
		+ Body.first + ":" + Body.second + "\n" + BodyLength.first + ":" + std::to_string(BodyLength.second) + "\n" + Time.first + ":" + std::to_string(Time.second) + "\n" + "\n";
};

//interpreting string and contruct message
void Msg::StringtoMsg(std::string str){
	Command = str.substr(0, str.find('\n'));
	str = str.substr(str.find('\n') + 1);
	while (true)
	{
		std::string line;
		int counter = str.find('\n');
		if (counter == 0)
			return;
		line = str.substr(0, counter);
		str = str.substr(counter + 1);
		std::string name = line.substr(0, line.find(':'));
		std::string value = line.substr(line.find(':') + 1);
		StrToMsghelper(name, value);
	}
};

void Msg::StrToMsghelper(std::string &name, std::string &value)
{
	if (name == "SrcIP")
		SrcIP.second = value;
	else if (name == "SrcPort")
		SrcPort.second = atoi(value.c_str());
	else if (name == "DestIP")
		DestIP.second = value;
	else if (name == "DestPort")
		DestPort.second = atoi(value.c_str());
	else if (name == "FileName")
		FileName.second = value;
	else if (name == "Body")
		Body.second = value;
	else if (name == "BodyLength")
		BodyLength.second = atoi(value.c_str());
	else if (name == "Time_Millisecond")
		Time.second = atoll(value.c_str());
	else
		Body.second = Body.second + value + "\n";
};

#ifdef TEST_MSG

int main()
{
	msg.construct_message("localhost", 8080, "localhost", 9081, "DEMO.TSET", "DEMO.BODY", 9);
	std::cout << "\n  Content of msg after construction:\n" + msg.MsgtoString() << std::endl;
	Msg msg2; msg2.StringtoMsg(msg.MsgtoString());
	std::cout << "\n  Content of msg2 by interperting string of msg:\n" + msg2.MsgtoString() << std::endl;
}

#endif
