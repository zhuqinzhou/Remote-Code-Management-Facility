/////////////////////////////////////////////////////////////////////////////
// Exec.cpp -  Executive methods of file catalog program                   //
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
#include "FileCatalog.h"
#include "FileMgr.h"
#include "DisplayPr1.h"
#include "FileSystem.h"
#include <iostream>
#include <iomanip>
//#define DEMO

using namespace FileManager;
using namespace DisplayPr1;
using namespace std;
using namespace FileSystem;

//======<Continue the text search after the construction of file catalog.>======
bool continuesearch(FileCatalog filecatalog)
{
	cout << "\n----- " << endl;
	string input;
	cout << "\nPlease input the file specifications for text searches. Input /x to exit the program.\ne.g. \"*.cpp\" \"*.*\" \"Exec.*\"" << endl;
	getline(cin, input);
	if (input == "/x")
		return false;
	cout << "\nPlease input the text you want to search. Input /x to exit the program." << endl;
	string searchtext;
	getline(cin, searchtext);
	if (searchtext == "/x")
		return false;
	vector<string> searchspecs;

	while (input.size() > 0)
	{
		int spacelocation = input.find_last_of(' ');
		string spec;
		if (spacelocation >= 0 && spacelocation < int(input.size()))
		{
			for (size_t i = spacelocation + 1; i < input.size(); ++i)
				spec.push_back(input[i]);
			if (spec != "")
				searchspecs.push_back(spec);
			input.resize(spacelocation);
		}
		else
		{
			searchspecs.push_back(input);
			break;
		}
	}
	if (searchspecs.size() == 0)
		searchspecs.push_back("*.*");

	cout << "\nSpecifications for text searches are: ";
	VectorLinePrint(searchspecs);
	if (searchtext.size() == 0)
		cout << "No searchtext specified, all files matching patterns will be listed:" << endl;
	else
		cout << "The search text is: \"" << searchtext << "\". \nText found in the following file:" << endl;

	TextSearchResult(FileManager::SearchText(filecatalog.PathList, filecatalog.FileList, searchtext, searchspecs));
	return true;
}

//======<Show the brief summary of file catalog.>======
void briefsummary(FileCatalog filecatalog)
{
	int filecount = 0;
	for each (FileEntry countfile in filecatalog.FileList)
	{
		filecount = filecount + countfile.filepathIndex.size();
	}
	cout << filecount << " file(s) found in " << filecatalog.PathList.size() << "Path(s)." << endl;
}

//======<Get options from user input.>======
void getoptions(bool &recursesearch, bool &showduplicate, bool &findtext, vector<string> &options, string fsearchtext)
{
	for each (string option in options)
	{
		string optionswitch = option.erase(2);
		if (option == "/s")
		{
			recursesearch = true;
			cout << "/s selected, search entire directory" << endl;
		}
		if (option == "/d")
		{
			showduplicate = true;
			cout << "/d selected, show duplicate names" << endl;
		}
		if (option == "/f")
		{
			findtext = true;
			cout << "/f selected, show files containing text: \"" << fsearchtext << "\"" << endl;
		}
	}
}

//======<Show result specified by options.>======
void showoptionresult(bool showduplicate, bool findtext, FileCatalog filecatalog, string fsearchtext, vector<string> patterns)
{
	//show the brief summary if no option was selected
	if (!showduplicate&&!findtext)
	{
		cout << "\n----- " << endl;
		briefsummary(filecatalog);
	}
	//show duplicate files
	if (showduplicate)
	{
		cout << "\n----- " << endl;
		cout << "\nDuplicate files are:" << endl;
		for each (FileEntry file in filecatalog.FileList)
		{
			if (file.filepathIndex.size() > 1)
			{
				cout << "  " << file.filename << ", found in these paths:" << endl;
				for each (int ind in file.filepathIndex)
				{
					cout << "    " << filecatalog.PathList[ind] << endl;
				}
			}
		}
	}
	//show search text result
	if (findtext)
	{
		cout << "\n----- " << endl;
		cout << "\nThe search text is: \"" << fsearchtext << "\".\nText found in the following file:" << endl;
		TextSearchResult(FileManager::SearchText(filecatalog.PathList, filecatalog.FileList, fsearchtext, patterns));
	}
}

//======<check whether the input path exists>======
void pathcheck(string startpath)
{
	cout << endl;
	while (!Directory::setCurrentDirectory(startpath))
	{
		cout << "Path does not exist, please input a valid path: ";
		cin >> startpath;
		cout << endl;
	}
	cout << "\nThe root directory is: " << Directory::getCurrentDirectory() << endl;
}

//======<Main function>======
#ifndef DEMO
int main(int argc, char *argv[])
{
	FileCatalog filecatalog;
	string startpath;
	vector<string> options;
	vector<string> patterns;
	string fsearchtext;
	if (argc > 1)
		startpath = argv[1];
	for (int i = 2; i < argc; ++i){
		if (argv[i][0] == '/'){
			options.push_back(argv[i]);
			if (argv[i][1] == 'f'){
				string tempstring = argv[i];
				for (size_t j = 2; j < tempstring.size(); ++j)
					fsearchtext.push_back(tempstring[j]);
			}
		}
		else
			patterns.push_back(argv[i]);
	}
	if (patterns.size() == 0)
		patterns.push_back("*.*");
	cout << "\nThe input was: ";
	for (int i = 1; i < argc; ++i)
		cout << argv[i] << ' ';
	pathcheck(startpath);
	cout << "\nFile pattenrs are: ";
	VectorLinePrint(patterns);
	cout << endl;
	bool recursesearch = false;
	bool showduplicate = false;
	bool findtext = false;
	getoptions(recursesearch, showduplicate, findtext, options, fsearchtext);
	DemonstrateFileDirectory(Directory::getCurrentDirectory(), patterns, recursesearch, filecatalog.PathList, filecatalog.FileList);
	showoptionresult(showduplicate, findtext, filecatalog, fsearchtext, patterns);
	//cin.ignore();
	while (continuesearch(filecatalog)){}
	return 0;
}
#endif

//======<Demonstration Utilities>======
#ifdef DEMO
void demo(FileCatalog &filecatalog, string startpath, vector<string> options, vector<string> patterns, string fsearchtext, bool continuesearch, string input)
{
	cout << "\nThe input was: " << input;
	pathcheck(startpath);
	cout << "\nFile pattenrs are: ";
	VectorLinePrint(patterns);
	cout << endl;
	bool recursesearch = false;
	bool showduplicate = false;
	bool findtext = false;
	getoptions(recursesearch, showduplicate, findtext, options, fsearchtext);
	DemonstrateFileDirectory(Directory::getCurrentDirectory(), patterns, recursesearch, filecatalog.PathList, filecatalog.FileList);
	showoptionresult(showduplicate, findtext, filecatalog, fsearchtext, patterns);
}

//======<Print each file in file store along with its paths>======
void printfiles(FileCatalog filecatalog)
{
	cout << "The files are: " << endl;
	for each (FileEntry file in filecatalog.FileList)
	{
		for each (int ind in file.filepathIndex)
			cout << "    " << filecatalog.PathList[ind] + '\\' + file.filename << endl;
	}
}

//======<Demo requirement3>======
void requirement3()
{
	FileCatalog filecatalog1, filecatalog2;
	cout << "-=-=-=-=-=-=-=-=-=-Demonstrating requirement 3-=-=-=-=-=-=-=-=-=-" << endl;
	cout << "// When /s was selected:" << endl;
	vector<string> options(1, "/s");
	vector<string> patterns;
	patterns.push_back("*.cpp"); patterns.push_back("*.txt");
	demo(filecatalog1, ("./"), options, patterns, (""), false, "./ /s *.cpp *.txt");
	printfiles(filecatalog1);
	cout << "====================\n\n// When /s was NOT selected:" << endl;
	options.clear(); patterns.clear(); patterns.push_back("*.h"); patterns.push_back("*.txt");
	demo(filecatalog2, ("./test"), options, patterns, (""), false, "./test *.h *.txt");
	printfiles(filecatalog2);
	cout << "====================\n\n//The result showed that the program could take a path, one or more file patterns, and switch /s properly." << endl;
}

//======<Demo requirement4>======
void requirement4()
{
	FileCatalog filecatalog1;
	cout << "-=-=-=-=-=-=-=-=-=-Demonstrating requirement 4-=-=-=-=-=-=-=-=-=-" << endl;
	vector<string> options(1, "/s");
	vector<string> patterns;
	patterns.push_back("*.log"); patterns.push_back("*.txt"); patterns.push_back("*.cpp");
	demo(filecatalog1, ("./"), options, patterns, (""), false, "./ /s *.log *.txt *.cpp");
	cout << "//Each path was saved only once in a vector:" << endl;
	cout << "======<Content in path storage>======" << endl;
	cout << "Index     Path" << endl;
	for (size_t i = 0; i < filecatalog1.PathList.size(); ++i)
		cout << setw(5) << i << "     " << filecatalog1.PathList[i] << endl;
	cout << "\n//Each file name was saved only once in a vector, alone with their path references." << endl;
	cout << "//Their path references are integer iterators, which indicates indices in path storage." << endl;
	cout << "//Each entry in the vector is a set of a file name and a list of references." << endl;
	cout << "======<Content in file & ref. storage>======" << endl;
	cout << setw(35) << "File Name" << "     " << "References" << endl;
	for (size_t i = 0; i < filecatalog1.FileList.size(); ++i)
	{
		cout << setw(35) << filecatalog1.FileList[i].filename << "     ";
		ListPrint(filecatalog1.FileList[i].filepathIndex);
		cout << endl;
	}
	cout << "====================\n\n//The storage class was defined in \"FileCatalog.h\". All names and paths were saved only once in the program. \n// Finishing demonstrating requirement 4." << endl;
}

//======<Demo requirement5>======
void requirement5()
{
	FileCatalog filecatalog1;
	cout << "-=-=-=-=-=-=-=-=-=-Demonstrating requirement 5-=-=-=-=-=-=-=-=-=-" << endl;
	cout << "// When /d was selected:" << endl;
	vector<string> options;
	options.push_back("/s"); options.push_back("/d");
	vector<string> patterns;
	patterns.push_back("*.*");
	demo(filecatalog1, ("./"), options, patterns, (""), false, "./ /s /d *.*");
	cout << "\n// Finishing demonstrating requirement 5." << endl;
}

//======<Demo requirement6>======
void requirement6()
{
	FileCatalog filecatalog1;
	cout << "-=-=-=-=-=-=-=-=-=-Demonstrating requirement6-=-=-=-=-=-=-=-=-=-" << endl;
	cout << "// When /f was selected, and search text is \"file1text\"" << endl;
	vector<string> options;
	options.push_back("/s"); options.push_back("/f");
	vector<string> patterns;
	patterns.push_back("*.*");
	demo(filecatalog1, ("./"), options, patterns, ("file1text"), false, "./ /s /ffile1text *.*");
	cout << "\n// When /f was selected, and search text is \"file2text\"" << endl;
	FileCatalog filecatalog2;
	demo(filecatalog2, ("./"), options, patterns, ("file2text"), false, "./ /s /ffile2text *.*");
	cout << "\n// Finishing demonstrating requirement 6." << endl;
}

//======<Demo requirement7>======
void requirement7()
{
	FileCatalog filecatalog1;
	cout << "-=-=-=-=-=-=-=-=-=-Demonstrating requirement7-=-=-=-=-=-=-=-=-=-" << endl;
	cout << "// When no option was selected:" << endl;
	vector<string> options(1, "/s");
	vector<string> patterns;
	patterns.push_back("*.*");
	demo(filecatalog1, ("./"), options, patterns, (""), false, "./ /s *.*");
	cout << "\n// The brief Summary is shown above. Finishing demonstrating requirement 7." << endl;
}

//======<Demo requirement8&9>======
void requirement8and9()
{
	FileCatalog filecatalog1;
	cout << "-=-=-=-=-=-=-=-=-=-Demonstrating requirement8 and requirement9-=-=-=-=-=-=-=-=-=-" << endl;
	cout << "\n// requirement 8 would be demonstrated after pressing ENTER;" << endl;
	cout << "//For requirement 9, please check \"Exec.cpp\" for executive package" << endl;
	cout << "//and \"Display.cpp\", \"Display.h\" for the display package in the project directory." << endl;
}

//======<Main Function for demo>======
int main()
{
	cout<<">>>>>>IMPORTANT<<<<<<"<<endl;
	cout<<"This demostration program was compiled with \"CL=/DDEMO\" option, which means it's generated specially for demonstration."<<endl;
	cout<<"In order to generate the executable program for general use, you MUST run \"run.bat\" once."<<endl;
	cout<<"user run 'compile.bat' => demostration program generated => user run 'run.bat' => demonstrating, applicable program generated."<<endl;
	cout<<"======DEMO START======"<<endl;
	requirement3();
	cin.ignore();
	Directory::setCurrentDirectory("..");
	requirement4();
	cin.ignore();
	requirement5();
	cin.ignore();
	requirement6();
	cin.ignore();
	requirement7();
	cin.ignore();
	requirement8and9();
	cin.ignore();
}
#endif