#ifndef DISPLAY_H
#define DISPLAY_H
/////////////////////////////////////////////////////////////////////////////
// Display.h -  Display methods designed for file catalog                  //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright ?Qinzhou Zhu, 2015                                            //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    C++, Visual Studio 2013                                    //
// Platform:    X230T, Core i5, Windows 7 SP1                              //
// Application: CSE687-OOD, Project 1, Spring 2015                         //
// Author:      Qinzhou Zhu, Syracuse University                           //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module was used to display the program result and demonstrate the file catalog program.
*
*
* Members:
* ==================
* void TextSearchResult(std::vector<std::string> resultlist):
*   -Display the text search result specified by /f<search text>.
*
* void ListPrint(std::vector<std::string> vector):
*   -Print all entries in a string vector to a line, with quotation marks
*
* void ListPrint(std::vector<int> vector):
*   -List all entries in a int vector.
*
* void VectorLinePrint(std::vector<std::string> vector):
*   -Print all entries in a string vector to a line.
*
*
* Required Files:
* ===============
* none
*
*
* Maintenance History:
* ====================
*  ver 1.0 : 10 Feb 15
* - first release
*/
#include <string>
#include <vector>
#include <iostream>

namespace DisplayPr1
{
	//======<Display the text search result specified by /f<search text>.>======
	void TextSearchResult(std::vector<std::string> resultlist);

	//======<Print all entries in a string vector to a line, with quotation marks>======
	void ListPrint(std::vector<std::string> vector);

	//======<List all entries in a int vector.>======
	void ListPrint(std::vector<int> vector);

	//======<Print all entries in a string vector to a line.>======
	void VectorLinePrint(std::vector<std::string> vector);
}
#endif