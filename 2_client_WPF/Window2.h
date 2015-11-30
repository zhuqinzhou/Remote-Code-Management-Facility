#ifndef WINDOW2_H
#define WINDOW2_H
///////////////////////////////////////////////////////////////////////////
// Window.h - C++\CLI implementation of WPF Application                  //
//          - Demo for CSE 687 Project #4                                //
// ver 2.1                                                               //
// Qinzhou Zhu, CSE687 - Object Oriented Design, Spring 2015             //
// Original Author:  Jim Fawcett, Prof. CSE687                           //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package demonstrates how to build a C++\CLI WPF application.  It
*  provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  The window class hosts, in its window, a tab control with three views, two
*  of which are provided with functionality that you may need for Project #4.
*  It loads the DLL holding MockChannel.  MockChannel hosts a send queue, a
*  receive queue, and a C++11 thread that reads from the send queue and writes
*  the deQ'd message to the receive queue.
*
*  The Client can post a message to the MockChannel's send queue.  It hosts
*  a receive thread that reads the receive queue and dispatches any message
*  read to a ListBox in the Client's FileList tab.  So this Demo simulates
*  operations you will need to execute for Project #4.
*
*  Required Files:
*  ---------------
*  Window.h, Window.cpp, MochChannel.h, MochChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
*  Maintenance History:
*  --------------------
*  ver 2.1 : 27 Apr 2015
*  - add textbox, button for my Pr4
*  ver 2.0 : 15 Apr 2015
*  - completed message passing demo with moch channel
*  - added FileBrowserDialog to show files in a selected directory
*  ver 1.0 : 13 Apr 2015
*  - incomplete demo with GUI but not connected to mock channel
*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     System
*     System.Windows.Presentation
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/
using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../MockChannel/MockChannel.h"
#include <iostream>

namespace CppCliWindows
{
	ref class WPFCppCliDemo : Window
	{
		//prompt when uploading file
		void waitupload(){
			while (*res == System::Windows::Forms::DialogResult::OK){
				*res = System::Windows::Forms::MessageBox::Show("Please wait for uploading file.", "Message",
					System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Information);
			}
		}

		// MockChannel references
		Iclient* pclient_;
		ISendr* pSendr_;
		IRecvr* pRecvr_;
		IMockChannel* pChann_;

		// Controls for Window

		DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
		Grid^ hGrid = gcnew Grid();
		TabControl^ hTabControl = gcnew TabControl();
		TabItem^ hSendMessageTab = gcnew TabItem();
		TabItem^ hFileListTab = gcnew TabItem();
		TabItem^ hConnectTab = gcnew TabItem();
		StatusBar^ hStatusBar = gcnew StatusBar();
		StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
		TextBlock^ hStatus = gcnew TextBlock();

		// Controls for SendMessage View

		Grid^ hSendMessageGrid = gcnew Grid();
		Button^ hSendButton = gcnew Button();
		Button^ hClearButton = gcnew Button();
		Button^ hDownLoadButton = gcnew Button();
		Button^ hUploadButton = gcnew Button();
		TextBox^ hTextBlock1 = gcnew TextBox();
		TextBox^ hTextBlock2 = gcnew TextBox();
		ScrollViewer^ hScrollViewer1 = gcnew ScrollViewer();
		ScrollViewer^ hScrollViewer2 = gcnew ScrollViewer();
		StackPanel^ hStackPanel1 = gcnew StackPanel();
		TextBox^ hpath = gcnew TextBox();
		TextBox^ hpatterns = gcnew TextBox();
		TextBox^ dlfullpath = gcnew TextBox();
		TextBox^ upfullpath = gcnew TextBox();
		TextBox^ searchtext = gcnew TextBox();

		String^ msgText
			= "Command:ShowMessage\n"   // command
			+ "Sendr:localhost@8080\n"  // send address
			+ "Recvr:localhost@8090\n"  // receive address
			+ "Content-length:44\n"     // body length attribute
			+ "\n"                      // end header
			+ "Hello World\nCSE687 - Object Oriented Design";  // message body

		// Controls for FileListView View

		Grid^ hFileListGrid = gcnew Grid();
		Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
		ListBox^ hListBox = gcnew ListBox();
		Button^ hFolderBrowseButton = gcnew Button();
		Grid^ hGrid2 = gcnew Grid();

		// Controls for connect

		CheckBox ^ connectserver1 = gcnew CheckBox();
		CheckBox ^ connectserver2 = gcnew CheckBox();
		CheckBox ^ getxml = gcnew CheckBox();

		// receive thread

		Thread^ recvThread;

	public:
		WPFCppCliDemo();
		~WPFCppCliDemo();

		void setUpStatusBar();
		void setUpTabControl();
		void setUpSendMessageView();
		void setUpFileListView();
		void setUpConnectionView();

		void startserver(Object^ obj, RoutedEventArgs^ args){};
		void sendMessage(Object^ obj, RoutedEventArgs^ args);
		void DownloadFile(Object^ obj, RoutedEventArgs^ args);
		void UploadFile(Object^ obj, RoutedEventArgs^ args);

		void addText(String^ msg);
		void getMessage();
		void displayMessage();
		void clear(Object^ sender, RoutedEventArgs^ args);
		void browseForFolder(Object^ sender, RoutedEventArgs^ args);
		void OnLoaded(Object^ sender, RoutedEventArgs^ args);
		void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
	private:

		System::Windows::Forms::DialogResult *res = new System::Windows::Forms::DialogResult(System::Windows::Forms::DialogResult::OK);

		std::string toStdString(String^ pStr);
		String^ toSystemString(std::string& str);
		void setTextBlockProperties();
		void setButtonsProperties();
	};
}


#endif
