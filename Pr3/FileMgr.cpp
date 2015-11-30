///////////////////////////////////////////////////////////////////////////////
// FileMgr.cpp -  Manage the implementation and construction of file catalog.//
// ver 1.0                                                                   //
// ------------------------------------------------------------------------- //
// copyright ?Qinzhou Zhu, 2015                                              //
// All rights granted provided that this notice is retained                  //
// ------------------------------------------------------------------------- //
// Language:    C++, Visual Studio 2013                                      //
// Platform:    X230T, Core i5, Windows 7 SP1                                //
// Application: CSE687-OOD, Project 1, Spring 2015                           //
// Author:      Qinzhou Zhu, Syracuse University                             //
///////////////////////////////////////////////////////////////////////////////
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
#include "FileMgr.h"
#include "FileSystem.h"
//#define TEST_FILEMGR

//=====<Construct the file catalog according to user input.>======
void FileManager::DemonstrateFileDirectory(const std::string& path, std::vector<std::string> &patterns, bool recursesearch, std::vector<std::string> &PathList, std::vector<FileEntry> &FileList)
{
	PathList.push_back(path);
	std::vector<std::string> currfiles;
	for each (std::string pattern in patterns)
	{
		currfiles = FileSystem::Directory::getFiles(path, pattern);
		for (size_t i = 0; i < currfiles.size(); ++i)
		{
			bool duplicatefile = false;
			for (size_t j = 0; j < FileList.size(); ++j)
			{
				if (FileList[j].filename == currfiles[i])
				{
					duplicatefile = true;
					FileList[j].filepathIndex.push_back(PathList.size() - 1);
					break;
				}
			}
			if (!duplicatefile)
			{
				FileEntry file;
				file.filename = currfiles[i];
				file.filepathIndex.push_back(PathList.size() - 1);
				FileList.push_back(file);
			}
		}
	}
	if (recursesearch)
	{
		std::vector<std::string> currdirs = FileSystem::Directory::getDirectories(path);
		for (size_t i = 0; i < currdirs.size(); ++i)
		{
			if (currdirs[i] != "."&&currdirs[i] != "..")
			{
				DemonstrateFileDirectory(path + '\\' + currdirs[i].c_str(), patterns, recursesearch, PathList, FileList);
			}
		}
	}
}

//======<Delete the extension in the file name>=======
std::string FileManager::deleteExtension(std::string filename)
{
	std::string Ext = FileSystem::Path::getExt(filename);
	filename.erase(filename.size() - Ext.size());
	if (filename.back() == '.')
	{
		filename.pop_back();
	}
	return filename;
}

//======<Check whether a file name could be matched with inputted specifications.>======
bool FileManager::patternmatch(std::string filename, std::vector<std::string>filespecs)
{
	for each (std::string spec in filespecs)
	{
		if (FileSystem::Path::getExt(spec) == "*" || FileSystem::Path::getExt(filename) == FileSystem::Path::getExt(spec))
		{
			filename = deleteExtension(filename);
			spec = deleteExtension(spec);
			{
				if (spec == "*" || filename == spec)
					return true;
			}
		}
	}
	return false;
}

//======<Search the user specified text in files contained in the file catalog.>======
std::vector<std::string> FileManager::SearchText(std::vector<std::string> &PathList, std::vector<FileEntry> &FileList, std::string text, std::vector<std::string>filespecs)
{
	std::vector<std::string> locationlist;
	for each (FileEntry file in FileList)
	{
		for (size_t i = 0; i < file.filepathIndex.size(); ++i)
		{
			if (patternmatch(file.filename, filespecs))
			{
				FileSystem::File in(PathList[file.filepathIndex[i]] + '\\' + file.filename);
				in.open(FileSystem::File::in);
				while (in.isGood())
				{
					std::string templine = in.getLine();
					size_t textlocation = templine.find(text);
					if (textlocation >= 0 && textlocation < templine.size())
					{
						locationlist.push_back(PathList[file.filepathIndex[i]] + '\\' + file.filename);
						break;
					}
				}
				in.close();
			}
		}
	}
	return locationlist;
}

//======<Test Stub>======
#ifdef TEST_FILEMGR
int main()
{
	std::vector<std::string> specs(1,"*.bar");
	std::cout << FileManager::deleteExtension("foo.bar") << std::endl;
	std::cout << FileManager::patternmatch("foobar",specs) << std::endl;
	return 0;
}
#endif