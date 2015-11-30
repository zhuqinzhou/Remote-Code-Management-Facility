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

#include "DisplayPr1.h"
//#define TEST_DISPLAY

//======<Display the text search result specified by /f<search text>.>======
void DisplayPr1::TextSearchResult(std::vector<std::string> resultlist)
{
	for each (std::string result in resultlist)
	{
		std::cout << "  " << result << std::endl;
	}
}

//======<List all entries in a string vector.>======
void DisplayPr1::ListPrint(std::vector<std::string> vector)
{
	for each(std::string entry in vector)
		std::cout << entry << " ";
}

//======<List all entries in a int vector.>======
void DisplayPr1::ListPrint(std::vector<int> vector)
{
	for each(int entry in vector)
		std::cout << entry << " ";
}

//======<Print all entries in a string vector to a line, with quotation marks>======
void DisplayPr1::VectorLinePrint(std::vector<std::string> vector)
{
	for each(std::string entry in vector)
	{
		std::cout << "\"" << entry << "\" ";
	}
	std::cout << std::endl;
}

//======<Test Stub>======
#ifdef TEST_DISPLAY
int main()
{
	std::vector<std::string> Vec;
	for (int i = 1; i < 11; ++i)
		Vec.push_back(std::to_string(i));
	Display::TextSearchResult(Vec);
	Display::VectorLinePrint(Vec);
	return 0;
}
#endif