/////////////////////////////////////////////////////////////////////////////
// Server.cpp - socket server                                              //
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
*  Server.h
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
#include "sockets.h"
#include "AppHelpers.h"
#include <iostream>
#include <thread>
#include <mutex>
#include "Sender.h"
#include "Receiver.h"
#include "Server.h"
#include "FileMgr.h"

void findrootpath(const std::string tag, std::string &xml, const std::string indent)
{
	if (tag == "." || tag == "./" || tag == ".\\"){
		std::string curr = FileSystem::Directory::getCurrentDirectory();
		curr = curr.substr(curr.find_last_of("\\") + 1);
		xml += '\n' + indent + "<Path name=\"" + curr + "\">";
	}
	else
		xml += '\n' + indent + "<Path name=\"" + tag + "\">";
}

void buildxml(std::string &xml, std::vector<std::string> &location, std::string indent)
{
	while (location.size() > 0)
	{
		std::string tag = location[0].substr(0, location[0].find("\\"));
		if (tag == location[0]){
			xml += '\n' + indent + "<File name=\"" + tag + "\"/>";
			location.erase(location.begin() + 0);
		}
		else{
			std::vector<std::string> newlocation;
			findrootpath(tag, xml, indent);
			for (size_t i = 0; i < location.size(); ++i)
			{
				if (location[i].substr(0, location[i].find("\\")) == tag)
				{
					newlocation.push_back(location[i].substr(location[i].find("\\") + 1));
					location.erase(location.begin() + i);
					--i;
				}
			}
			buildxml(xml, newlocation, indent + "  ");
			xml += '\n' + indent + "</Path>";
		}
	}
}
//Get path patterns from message;
void GetPathPatterns(Msg &message, std::string &path, std::vector<std::string> &patterns)
{
	int split = message.fileName().find("<=>");
	path = message.fileName().substr(0, split);
	std::string temp = message.fileName().substr(split + 3);

	std::string buf;
	std::stringstream ss(temp);

	while (ss >> buf)
	{
		patterns.push_back(buf);
	}
	if (path.size() == 0){ path = "."; }
	if (patterns.size() == 0){ patterns.push_back("*.*"); }
};

//server constructor
Server::Server(int port, Socket::IpVer IpVer){
	ApplicationHelpers::Verbose v(false);
	sndr = new Sender();
	StartListen(port, IpVer);
};

//server strat listening to port
void Server::StartListen(int port, Socket::IpVer IpVer){
	rcvr.StartListen(port, IpVer, true);
	std::thread t1([&](){
		while (true){
			Msg msg = this->RecvMsg();
			if (msg.command() == "_EXIT")
				break;
			msg.reverse();
			__int64 t1 = GetMSTime();
			if (msg.command() == "DownLoadFile")
			{
				msg.command("UpLoadFile");
				bool success = this->SendBin(msg);
				if (!success)
				{
					msg.command("UpLoadFailed");
					this->SendMsg(msg);
				}
			}
			else if (msg.command() == "GetFiles")
			{
				GetFilelist(msg);
				this->SendMsg(msg);
			}
			else
				this->SendMsg(msg);
			__int64 t2 = GetMSTime();
			std::cout << "\n  Processing Time(millisecond) = " + std::to_string(t2 - t1);
		}
	});
	t1.detach();
};
//server stop listen
void Server::StopListen(){ delete sndr; }

// send a message from the server
void Server::SendMsg(Msg msg){ std::cout << "\n\n  Send Message: " + msg.command();  sndr->SendMsg(msg); };

//send a file from server
bool Server::SendBin(Msg msg){ std::cout << "\n\n  Send Message: " + msg.command(); return sndr->SendBin(msg); };

//get file list
void Server::GetFilelist(Msg &msg)
{
	std::vector<std::string> PathList;
	std::vector<FileEntry> FileList;
	std::vector<std::string> patterns;
	std::string path;
	GetPathPatterns(msg, path, patterns);
	FileManager::DemonstrateFileDirectory(path, patterns, true, PathList, FileList);
	std::string result;
	for (size_t i = 0; i < FileList.size(); ++i)
	{
		for (size_t j = 0; j < FileList[i].filepathIndex.size(); ++j)
			result = result + "\n" + PathList[FileList[i].filepathIndex[j]] + "\\" + FileList[i].filename;
	}
	//socket_.sendString(result);

	result = "";
	std::vector<std::string> location = FileManager::SearchText(PathList, FileList, msg.body(), patterns);
	if (msg.bodyLength() == 0)
	{
		for (size_t i = 0; i < location.size(); ++i){
			result = result + "\n" + location[i];
		}
	}
	else if (msg.bodyLength() == 1)
	{
		std::string xml = "";
		std::string indent = "";
		buildxml(xml, location, indent);
		std::string title = "\n<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		result = title + xml;
	}
	msg.command("GetFilesResult");
	msg.body(result);
};

//server exit
void Server::eixt(Msg msg){ rcvr.eixt(msg); };

#ifdef SERVER_CLIENT

int main()
{
	HWND hwnd = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(hwnd, FALSE);
	DeleteMenu(hmenu, SC_CLOSE, MF_GRAYED);
	title("Testing Server: 1_server", '=');
	//server at localhost:9080
	Server server(9080, Socket::IpVer::IP4);
	term_count terminate;
	std::this_thread::sleep_for(std::chrono::seconds(3));
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
	std::cout << "\n\n";
}

#endif
