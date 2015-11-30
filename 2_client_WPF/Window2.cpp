///////////////////////////////////////////////////////////////////////////
// Window2.cpp - C++\CLI implementation of WPF Application                //
//          -  for CSE 687 Project #4 Client 1 program                   //
// ver 1.0                                                               //
// Qinzhou Zhu, CSE687 - Object Oriented Design, Spring 2015             //
// Original Author:  Jim Fawcett, Prof. CSE687                           //
///////////////////////////////////////////////////////////////////////////
/*
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*
*  Package Operations:
*  -------------------
*  This is the client 1 implementation of Pr4. Url=localhost:8081
*
*  Required Files:
*  ---------------
*  Window2.h, msclr\marshal_cppstd.h
*  Window2.h, chrono
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 27 Apr 2015
*  - First Build
*/
#include <Windows.h>
#include <msclr\marshal_cppstd.h>
#include "Window2.h"
#include <chrono>
using namespace CppCliWindows;

//get the current system time
__int64 GetMSTime()
{
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	__int64 time = std::chrono::duration_cast<std::chrono::milliseconds>(t1.time_since_epoch()).count();
	return time;
}

//build the overall GUI for client program
WPFCppCliDemo::WPFCppCliDemo(){
	// set up channel
	ObjectFactory* pObjFact = new ObjectFactory;
	pSendr_ = pObjFact->createSendr();
	pRecvr_ = pObjFact->createRecvr();
	pclient_ = pObjFact->createclient(8081);
	pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);
	pChann_->start();
	delete pObjFact;
	// client's receive thread
	recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
	recvThread->Start();
	// set event handlers
	this->Loaded += gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing += gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	hSendButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendMessage);
	hClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
	hDownLoadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DownloadFile);
	hUploadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::UploadFile);
	hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
	// set Window properties
	this->Title = "Client2(localhost:8081)"; this->Width = 800; this->Height = 600;
	// attach dock panel to Window
	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);
	// setup Window controls and views
	setUpTabControl(); setUpStatusBar(); setUpSendMessageView(); setUpFileListView(); setUpConnectionView();
	Thread^ displayThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::displayMessage));
	displayThread->Start();
}

//destructor
WPFCppCliDemo::~WPFCppCliDemo()
{
	delete pChann_;
	delete pSendr_;
	delete pRecvr_;
	delete pclient_;
}

//set up the status bar at the bottom
void WPFCppCliDemo::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = "very important messages will appear here";
	//status->FontWeight = FontWeights::Bold;
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

//set up the tab on GUI
void WPFCppCliDemo::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hSendMessageTab->Header = "Send Message";
	hFileListTab->Header = "File List";
	hConnectTab->Header = "Connect";
	hTabControl->Items->Add(hConnectTab);
	hTabControl->Items->Add(hSendMessageTab);
	hTabControl->Items->Add(hFileListTab);

}

//set up text block from communication tab
void WPFCppCliDemo::setTextBlockProperties()
{
	StackPanel^ hStackPanel1 = gcnew StackPanel();
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	ColumnDefinition^ hCol1Def = gcnew ColumnDefinition();
	ColumnDefinition^ hCol2Def = gcnew ColumnDefinition();
	hSendMessageGrid->RowDefinitions->Add(hRow1Def);
	hSendMessageGrid->ColumnDefinitions->Add(hCol1Def);
	hSendMessageGrid->ColumnDefinitions->Add(hCol2Def);
	Border^ hBorder1 = gcnew Border();
	Border^ hBorder2 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBlock1;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hTextBlock2;
	TextBlock^ hSpacer = gcnew TextBlock();
	hTextBlock1->Padding = Thickness(15);
	hTextBlock1->Text = "";
	hTextBlock1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlock1->FontWeight = FontWeights::Bold;
	hTextBlock1->FontSize = 14;
	hTextBlock1->IsReadOnly = true;
	hTextBlock2->Padding = Thickness(15);
	hTextBlock2->Text = "";
	hTextBlock2->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlock2->FontWeight = FontWeights::Bold;
	hTextBlock2->FontSize = 14;
	hTextBlock2->IsReadOnly = true;
	TextBlock^ hSpacer_textblock = gcnew TextBlock();
	hSpacer->Width = 10;
	hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Visible;
	hScrollViewer1->HorizontalScrollBarVisibility = ScrollBarVisibility::Visible;
	hScrollViewer1->Content = hBorder1;
	hScrollViewer2->VerticalScrollBarVisibility = ScrollBarVisibility::Visible;
	hScrollViewer2->HorizontalScrollBarVisibility = ScrollBarVisibility::Visible;
	hScrollViewer2->Content = hBorder2;
	hTextBlock1->Text = "Message from Server1:\n\n";
	hTextBlock2->Text = "Message from Server2:\n\n";
	hSendMessageGrid->SetRow(hScrollViewer1, 0);
	hSendMessageGrid->SetColumn(hScrollViewer1, 0);
	hSendMessageGrid->SetColumn(hScrollViewer2, 1);
	hSendMessageGrid->Children->Add(hScrollViewer1);
	hSendMessageGrid->Children->Add(hScrollViewer2);
}

//set up buttons properties for communication tab
void WPFCppCliDemo::setButtonsProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition(); hRow2Def->Height = GridLength(40);
	RowDefinition^ hRow3Def = gcnew RowDefinition(); hRow3Def->Height = GridLength(40);
	RowDefinition^ hRow4Def = gcnew RowDefinition(); hRow4Def->Height = GridLength(40);
	hSendMessageGrid->RowDefinitions->Add(hRow2Def); hSendMessageGrid->RowDefinitions->Add(hRow3Def); hSendMessageGrid->RowDefinitions->Add(hRow4Def);
	Border^ hBorder2 = gcnew Border(); hBorder2->Width = 120; hBorder2->Height = 30; hSendButton->Content = "Get File List"; hBorder2->Child = hSendButton;
	Border^ hBorder3 = gcnew Border(); hBorder3->Width = 120; hBorder3->Height = 30; hClearButton->Content = "Clear"; hBorder3->Child = hClearButton;
	hStackPanel1->Children->Add(hBorder2);
	TextBlock^ hSpacer = gcnew TextBlock(); hSpacer->Height = 20; hSpacer->FontSize = 14; hSpacer->Text = "  Path: ";
	Border^ hBorder31 = gcnew Border(); hBorder31->Width = 120; hBorder31->Height = 30; hBorder31->Child = hDownLoadButton; hDownLoadButton->Content = "Download File";
	TextBlock^ hSpacer311 = gcnew TextBlock(); hSpacer311->Height = 20; hSpacer311->FontSize = 14; hSpacer311->Text = "  Path+Name: ";
	Border^ hBorderdload = gcnew Border(); hBorderdload->Width = 120; hBorderdload->Height = 25;
	hBorderdload->BorderThickness = Thickness(1); hBorderdload->BorderBrush = Brushes::Black; hBorderdload->Child = dlfullpath;
	Border^ hBorder32 = gcnew Border(); hBorder32->Width = 120; hBorder32->Height = 30; hBorder32->Child = hUploadButton; hUploadButton->Content = "Upload File";
	TextBlock^ hSpacer321 = gcnew TextBlock(); hSpacer321->Height = 20; hSpacer321->FontSize = 14; hSpacer321->Text = "  Path+Name: ";
	Border^ hBorderupload = gcnew Border(); hBorderupload->Width = 120; hBorderupload->Height = 25;
	hBorderupload->BorderThickness = Thickness(1); hBorderupload->BorderBrush = Brushes::Black; hBorderupload->Child = upfullpath;
	Border^ hBorder4 = gcnew Border(); hBorder4->Width = 120; hBorder4->Height = 25;
	hBorder4->BorderThickness = Thickness(1); hBorder4->BorderBrush = Brushes::Black; hBorder4->Child = hpath;
	Border^ hBorder42 = gcnew Border(); hBorder42->Width = 120; hBorder42->Height = 25;
	hBorder42->BorderThickness = Thickness(1); hBorder42->BorderBrush = Brushes::Black; hBorder42->Child = hpatterns;
	TextBlock^ hSpacer2 = gcnew TextBlock(); hSpacer2->Height = 20; hSpacer2->FontSize = 14; hSpacer2->Text = "  Patterns: ";
	TextBlock^ hSpacersearchtext = gcnew TextBlock(); hSpacersearchtext->Height = 20; hSpacersearchtext->FontSize = 14; hSpacersearchtext->Text = "Search Text: ";
	TextBlock^ hSpacer3 = gcnew TextBlock(); hSpacer3->Width = 10;
	TextBlock^ hSpacer4 = gcnew TextBlock(); hSpacer4->Width = 20;
	TextBlock^ hSpacer5 = gcnew TextBlock(); hSpacer5->Width = 10;
	Border^ hBorder51 = gcnew Border(); hBorder51->Width = 120; hBorder51->Height = 25;
	hBorder51->BorderThickness = Thickness(1); hBorder51->BorderBrush = Brushes::Black; hBorder51->Child = searchtext;
	getxml->Content = "XML?";
	hStackPanel1->Children->Add(getxml); hStackPanel1->Children->Add(hSpacer); hStackPanel1->Children->Add(hBorder4); hStackPanel1->Children->Add(hSpacer2);
	hStackPanel1->Children->Add(hBorder42); hStackPanel1->Children->Add(hSpacer5); hStackPanel1->Children->Add(hSpacersearchtext); hStackPanel1->Children->Add(hBorder51);
	StackPanel^ hStackPanel2 = gcnew StackPanel(); StackPanel^ hStackPanel3 = gcnew StackPanel();
	hStackPanel3->Children->Add(hBorder3); hStackPanel2->Children->Add(hBorder31); hStackPanel2->Children->Add(hSpacer311); hStackPanel2->Children->Add(hBorderdload);
	hStackPanel2->Children->Add(hSpacer4); hStackPanel2->Children->Add(hBorder32); hStackPanel2->Children->Add(hSpacer321); hStackPanel2->Children->Add(hBorderupload);
	hStackPanel1->Orientation = Orientation::Horizontal; hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hStackPanel2->Orientation = Orientation::Horizontal; hStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hStackPanel3->Orientation = Orientation::Horizontal; hStackPanel3->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hSendMessageGrid->SetRow(hStackPanel1, 1); hSendMessageGrid->SetColumnSpan(hStackPanel1, 2);
	hSendMessageGrid->SetRow(hStackPanel2, 2); hSendMessageGrid->SetColumnSpan(hStackPanel2, 2);
	hSendMessageGrid->SetRow(hStackPanel3, 3); hSendMessageGrid->SetColumnSpan(hStackPanel3, 2);
	hSendMessageGrid->Children->Add(hStackPanel1);
	hSendMessageGrid->Children->Add(hStackPanel2);
	hSendMessageGrid->Children->Add(hStackPanel3);
}

//set up the overall communication tab
void WPFCppCliDemo::setUpSendMessageView()
{
	//Console::Write("\n  setting up sendMessage view");
	hSendMessageGrid->Margin = Thickness(20);
	hSendMessageTab->Content = hSendMessageGrid;

	setTextBlockProperties();
	setButtonsProperties();
}

//transfer system::string to std::string
std::string WPFCppCliDemo::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}

//set action for "Get File List" button
void WPFCppCliDemo::sendMessage(Object^ obj, RoutedEventArgs^ args)
{
	if (!connectserver1->IsChecked&&!connectserver2->IsChecked)
	{
		hStatus->Text = "Please connect to at least 1 server";
		return;
	}

	Msg msg;
	msg.command("GetFiles");
	msg.srcIP("127.0.0.1");
	msg.srcPort(8081);

	std::string str1 = msclr::interop::marshal_as<std::string>(hpath->Text);
	std::string str2 = msclr::interop::marshal_as<std::string>(hpatterns->Text);
	msg.fileName(str1 + "<=>" + str2);
	msg.body(msclr::interop::marshal_as<std::string>(searchtext->Text));
	if (getxml->IsChecked)
		msg.bodyLength(1);
	else
		msg.bodyLength(0);
	if (connectserver1->IsChecked){
		msg.byname("server1");
		msg.time(GetMSTime());
		pclient_->SendMsg(msg);
	}
	if (connectserver2->IsChecked){
		msg.byname("server2");
		msg.time(GetMSTime());
		pclient_->SendMsg(msg);
	}

	//pSendr_->postMessage(pclient_->GetMsg().MsgtoString());
	Console::Write("\n  Sent message");
	hStatus->Text = "Sent message";
}

//set action for "Download file" button
void WPFCppCliDemo::DownloadFile(Object^ obj, RoutedEventArgs^ args)
{
	if ((!connectserver1->IsChecked&&!connectserver2->IsChecked) || (connectserver1->IsChecked&&connectserver2->IsChecked)){
		hStatus->Text = "Please connect 1 and only 1 server";
		return;
	}
	Msg msg;
	msg.srcIP("127.0.0.1");
	msg.srcPort(8081);
	msg.command("DownLoadFile");
	std::string str1 = msclr::interop::marshal_as<std::string>(dlfullpath->Text);
	msg.fileName(str1);
	if (connectserver1->IsChecked){
		msg.byname("server1");
		msg.time(GetMSTime());
		pclient_->SendMsg(msg);
	}
	if (connectserver2->IsChecked){
		msg.byname("server2");
		msg.time(GetMSTime());
		pclient_->SendMsg(msg);
	}
	Console::Write("\n  Download File");
	hStatus->Text = "Download File";
}

//set action for "Upload file" button
void WPFCppCliDemo::UploadFile(Object^ obj, RoutedEventArgs^ args)
{
	if ((!connectserver1->IsChecked&&!connectserver2->IsChecked) || (connectserver1->IsChecked&&connectserver2->IsChecked)){
		hStatus->Text = "Please connect 1 and only 1 server";
		return;
	}
	Msg msg; msg.srcIP("127.0.0.1"); msg.srcPort(8081); msg.command("UpLoadFile");
	std::string str1 = msclr::interop::marshal_as<std::string>(upfullpath->Text);
	msg.fileName(str1);
	if (connectserver1->IsChecked){
		msg.byname("server1");
		ThreadStart ^start = gcnew ThreadStart(this, &WPFCppCliDemo::waitupload); Thread ^pThread = gcnew Thread(start);
		pThread->Start();
		msg.time(GetMSTime());
		bool success = pclient_->SendBin(msg);
		if (!success){
			msg.command("UpLoadFailed"); msg.reverse();
			__int64 a = GetMSTime(); msg.time(a - msg.time()); pSendr_->postMessage(msg.MsgtoString());
		}
		HWND hWnd = ::FindWindow(NULL, "Message");
		if (hWnd){
			::PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
		pThread->Abort();
	}
	if (connectserver2->IsChecked){
		msg.byname("server2");
		msg.srcIP("127.0.0.1");
		msg.srcPort(8081);
		msg.command("UpLoadFile");
		ThreadStart ^start = gcnew ThreadStart(this, &WPFCppCliDemo::waitupload); Thread ^pThread = gcnew Thread(start);
		pThread->Start();
		msg.time(GetMSTime());
		bool success = pclient_->SendBin(msg);
		if (!success){
			msg.command("UpLoadFailed"); msg.reverse();
			__int64 a = GetMSTime(); msg.time(a - msg.time()); pSendr_->postMessage(msg.MsgtoString());
		}
		HWND hWnd = ::FindWindow(NULL, "Message");
		if (hWnd){
			::PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
		pThread->Abort();
	}
	Console::Write("\n  Upload File");
	hStatus->Text = "Upload File";
}

//transfer std::string to system::string
String^ WPFCppCliDemo::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}

// add text to text block
void WPFCppCliDemo::addText(String^ msg)
{
	Msg message;
	std::string str = msclr::interop::marshal_as<std::string>(msg);
	message.StringtoMsg(str);

	if (message.srcPort() == 9080)
		hTextBlock1->Text += msg + "\n\n";
	if (message.srcPort() == 9081)
		hTextBlock2->Text += msg + "\n\n";
}

//get message from mockchannel receiver blockingQ
void WPFCppCliDemo::getMessage()
{
	// recvThread runs this function

	while (true)
	{
		//std::cout << "\n  receive thread calling getMessage()";
		std::string msg = pRecvr_->getMessage();
		String^ sMsg = toSystemString(msg);
		array<String^>^ args = gcnew array<String^>(1);
		args[0] = sMsg;

		Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
		Dispatcher->Invoke(act, args);  // must call addText on main UI thread
	}
}

//display message on the text block
void WPFCppCliDemo::displayMessage()
{
	// displayThread runs this function
	while (true){
		Msg msg = pclient_->GetMsg();
		__int64 a = GetMSTime();
		msg.time(a - msg.time());
		pSendr_->postMessage(msg.MsgtoString());
	}
}

//clear content shown on the text block
void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";
	hTextBlock1->Text = "Message from Server1:\n\n";
	hTextBlock2->Text = "Message from Server2:\n\n";
}

//set up the tab for getting file list
void WPFCppCliDemo::setUpFileListView()
{
	//Console::Write("\n  setting up FileList view");
	hFileListGrid->Margin = Thickness(20);
	hFileListTab->Content = hFileListGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	//hRow1Def->Height = GridLength(75);
	hFileListGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hListBox;
	hFileListGrid->SetRow(hBorder1, 0);
	hFileListGrid->Children->Add(hBorder1);

	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hFileListGrid->RowDefinitions->Add(hRow2Def);
	hFolderBrowseButton->Content = "Select Directory";
	hFolderBrowseButton->Height = 30;
	hFolderBrowseButton->Width = 120;
	hFolderBrowseButton->BorderThickness = Thickness(2);
	hFolderBrowseButton->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(hFolderBrowseButton, 1);
	hFileListGrid->Children->Add(hFolderBrowseButton);

	hFolderBrowserDialog->ShowNewFolderButton = false;
	hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}

//set action for browsing button
void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	hListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hFolderBrowserDialog->SelectedPath;
		std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		for (int i = 0; i < files->Length; ++i)
			hListBox->Items->Add(files[i]);
		array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
		for (int i = 0; i < dirs->Length; ++i)
			hListBox->Items->Add(L"<> " + dirs[i]);
	}
}

//set up the tab for connecting server
void WPFCppCliDemo::setUpConnectionView()
{
	//Console::Write("\n  setting up Connection view");
	Grid^ hsetUpConnectionGrid = gcnew Grid();
	hsetUpConnectionGrid->Margin = Thickness(20);
	hConnectTab->Content = hsetUpConnectionGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hsetUpConnectionGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	connectserver1->Content = "Connect Server1";
	hBorder2->Child = connectserver1;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	connectserver2->Content = "Connect Server2";
	hBorder3->Child = connectserver2;
	Button^ closeclient = gcnew Button();
	closeclient->Content = "Stop Listen";
	StackPanel^ hStackPanelc = gcnew StackPanel();
	hStackPanelc->Children->Add(hBorder2);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10;
	hStackPanelc->Children->Add(hSpacer);
	hStackPanelc->Children->Add(hBorder3);
	hStackPanelc->Children->Add(hSpacer2);
	hStackPanelc->Orientation = Orientation::Horizontal;
	hStackPanelc->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hsetUpConnectionGrid->SetRow(hStackPanelc, 0);
	hsetUpConnectionGrid->Children->Add(hStackPanelc);

}

//prompt when window loaded
void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("  Window loaded");
}

//prompt when window closed
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
	pclient_->StopListen();
}

//safe exit method
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
};

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)){
		Console::WriteLine(L"\n  Starting Client: 2_client");
		Console::WriteLine(L" ===========================");
		Application^ app = gcnew Application();
		app->Run(gcnew WPFCppCliDemo());
		Console::WriteLine(L"\n\n");
	}
}