/////////////////////////////////////////////////////////////////////////////
// Exec_servcer1.cpp - demonstrating server functions                      //
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
*  This package is the test executive package for server1 at localhost:9080
*
*  Required Files:
*  ---------------
*  ClientTest.cpp, ServerTest.cpp
*  Sockets.h, Sockets.cpp, AppHelpers.h, AppHelpers.cpp
*  Server.h
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
#include "../Pr3/Sockets.h"
#include "../Pr3/AppHelpers.h"
#include "../Pr3/Server.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <signal.h>

using namespace ApplicationHelpers;

//class for handling unexpected termination
class term_count
{
public:
	sig_atomic_t c_count(){ return ctrlCcount; }
	void add(){ ctrlCcount++; }
private:
	volatile sig_atomic_t ctrlCcount = 0;
};

//catch illegal termination
void catch_int(int sig_num){
	term_count ccount;
	ccount.add();
};

BOOL CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType){
	case CTRL_C_EVENT:
		printf("\nCtrl-C event\n\n");
		return(TRUE);
	case CTRL_CLOSE_EVENT:
		printf("\nCtrl-Close event\n\n");
		return(TRUE);
	default:
		return FALSE;
	}
}

//main function
int main()
{
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)){
		title("Starting Server: 1_server", '=');
		//server at localhost:9080
		Server server(9080, Socket::IpVer::IP4);
		term_count terminate;
		std::cout << "\n  press key to exit: ";
		std::cin.ignore();
		server.StopListen();
		std::cout << "\n\n";
	}
}

