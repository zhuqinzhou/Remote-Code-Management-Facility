#ifndef FILECATALOG_H
#define FILECATALOG_H
/////////////////////////////////////////////////////////////////////////////
// FileCatalog.h -  Catalog of all files in the file set                   //
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
 * This module provided the storage for file catalog of all files in the file set.
 *
 * The design goal was to save each file name only once and saving each path only once,
 * while preserving all of the containment relationships between directories and their files.
 * It implies that each file storage will have to save a list of references into a set of paths where they are found.
 *
 * In order to achieve this goal. The author defined 2 structs: FileEntry and FileCatalog.
 *
 *   FileCatalog contained:
 *     -std::vector<FileEntry> FileList: A vector list of file entries, each of which hold a file name and references
 *										 to path(s) where it was found. Each file name appeared only once.
 *	   -std::vector<std::string>: A vector list of path names. Each path name appeared only once.
 *
 *   FileEntry contained:
 *     -std::string filename: This was the name of a file.
 *     -std::vector<int> filepathIndex: which was a list of references to paths where the filename was found.
 *                                      Since paths were stored in a vector, integer iterators would do the job.
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

//======<Define the entries of the file list.>======
struct FileEntry
{
	std::string filename;
	std::vector<int> filepathIndex;
};

//======<Store file names and paths in the file catalog.>======
struct FileCatalog
{
	std::vector<FileEntry> FileList;
	std::vector<std::string> PathList;
};

#endif