/////////////////////////////////////////////////////////////////////////////
// Display.h - display package                                             //
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
*  None
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
#ifndef DISPLAY_H
#define DISPLAY_H
#include <iostream>

using namespace std;

//display class for demonstrating requirement
class Demo
{
public:
	//demo requirement3,6,7
	void DemoReq3n6n7();
	//demo requirement4
	void DemoReq4();
	//demo requirement5
	void DemoReq5();
	//demo requirement6,8
	void DemoReq6n8();
};

#endif