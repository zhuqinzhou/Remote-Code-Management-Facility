#ifndef FILEMGR_H
#define FILEMGR_H
/////////////////////////////////////////////////////////////////////////////
// FileMgr.h -  Manage the implementation and construction of file catalog.//
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
* This module was used to manage the implementation and construction of file catalog.
* 
*
* Members:
* ==================
*   void DemonstrateFileDirectory(......):
*     -Construct the file catalog according to user input.
*   
*	std::string deleteExtension(std::string filename):
*     -Delete the extension in the file name. 
*		e.g. if "filename.ext" was input, "filename" would be returned.
*
*   bool patternmatch(std::string filename, std::vector<std::string>filespecs):
*     -Check whether a file name could be matched with inputted specifications.
*		e.g. if filename="foo.bar", and if "foo.*" or "*.bar" is in filespecs, 
*		function would return true; if not, return false.
* 
*   std::vector<std::string> SearchText(......):
*     -Search the user specified text in files contained in the file catalog.
*
*
* Required Files:
* ===============
* FileSystem.h, FileCatalog.h
*
*
* Maintenance History:
* ====================
*  ver 1.0 : 10 Feb 15
* - first release
*/

#include "FileCatalog.h"
#include <string>
#include <vector>
#include <iostream>

namespace FileManager
{
	//=====<Construct the file catalog according to user input.>======
	void DemonstrateFileDirectory(const std::string& path, std::vector<std::string> &patterns, bool recursesearch, std::vector<std::string> &PathList, std::vector<FileEntry> &FileList);

	//======<Delete the extension in the file name>=======
	std::string deleteExtension(std::string filename);

	//======<Check whether a file name could be matched with inputted specifications.>======
	bool patternmatch(std::string filename, std::vector<std::string>filespecs);

	//======<Search the user specified text in files contained in the file catalog.>======
	std::vector<std::string> SearchText(std::vector<std::string> &PathList, std::vector<FileEntry> &FileList, std::string text, std::vector<std::string>filespecs);
}
#endif