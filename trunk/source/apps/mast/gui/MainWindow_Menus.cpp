/***
 * mast: MainWindow_Menus.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MainWindow.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include <wx/menu.h>
#include <wx/msgdlg.h>

#include <common/exceptions/Exception.h>
#include <common/io/files/VolumeLoader.h>
#include <mast/gui/dialogs/VolumeChooserDialog.h>
#include <mast/util/IOUtil.h>
#include <mast/util/StringConversion.h>

namespace mp {

//#################### LOCAL CONSTANTS ####################
enum
{
	MENUID_BASE = wxID_HIGHEST,		// a dummy value which is never used: subsequent values are guaranteed to be higher than this
	MENUID_FILE_EXIT,
	MENUID_FILE_OPEN,
	MENUID_FILE_OPEN_DICOMDIR,
	MENUID_FILE_OPEN_MODEL,
	MENUID_FILE_OPEN_VOLUMECHOICE,
	MENUID_FILE_REPLACE_VOLUMECHOICESECTION,
	MENUID_FILE_SAVE_MODEL,
	MENUID_FILE_SAVE_VOLUMECHOICE,
	MENUID_HELP_ABOUT,
};

//#################### PRIVATE METHODS ####################
void MainWindow::setup_menus()
{
	wxMenu *fileMenu = new wxMenu;
	wxMenu *fileOpenMenu = new wxMenu;
	fileMenu->AppendSubMenu(fileOpenMenu, wxT("&Open"));
		fileOpenMenu->Append(MENUID_FILE_OPEN_DICOMDIR, wxT("&DICOMDIR...\tCtrl+Shift+O"));
		fileOpenMenu->Append(MENUID_FILE_OPEN_MODEL, wxT("&Model...\tCtrl+O"));
		fileOpenMenu->Append(MENUID_FILE_OPEN_VOLUMECHOICE, wxT("&Volume Choice...\tCtrl+Alt+O"));
	wxMenu *fileSaveMenu = new wxMenu;
	fileMenu->AppendSubMenu(fileSaveMenu, wxT("&Save"));
		fileSaveMenu->Append(MENUID_FILE_SAVE_MODEL, wxT("&Model...\tCtrl+S"));
		fileSaveMenu->Append(MENUID_FILE_SAVE_VOLUMECHOICE, wxT("&Volume Choice...\tCtrl+Alt+S"));
	fileMenu->AppendSeparator();
	fileMenu->Append(MENUID_FILE_REPLACE_VOLUMECHOICESECTION, wxT("&Replace Volume Choice Section..."));
	fileMenu->AppendSeparator();
	fileMenu->Append(MENUID_FILE_EXIT, wxT("E&xit\tAlt+F4"));

	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(MENUID_HELP_ABOUT, wxT("&About...\tF2"));

	m_menuBar = new wxMenuBar;
	m_menuBar->Append(fileMenu, wxT("&File"));
	m_menuBar->Append(helpMenu, wxT("&Help"));

	SetMenuBar(m_menuBar);
}

//#################### EVENT HANDLERS ####################

//~~~~~~~~~~~~~~~~~~~~ MENUS ~~~~~~~~~~~~~~~~~~~~
void MainWindow::OnMenuFileExit(wxCommandEvent&)
{
	Close();
}

void MainWindow::OnMenuFileOpenDICOMDIR(wxCommandEvent&)
{
	wxFileDialog_Ptr dialog = construct_open_dialog(this, "Open DICOMDIR", "DICOMDIR Files|DICOMDIR");
	if(dialog->ShowModal() == wxID_OK)
	{
		std::string path = wxString_to_string(dialog->GetPath());
		try
		{
			check_file_exists(path);

			// Display a volume chooser dialog to allow the user to choose which volume to load.
			VolumeChooserDialog dialog(path);
			dialog.ShowModal();

			if(dialog.volume_choice())
			{
				VolumeLoader_Ptr loader(new VolumeLoader);
				boost::thread volumeLoaderThread(boost::bind(&MainWindow::volume_loader_thread, this, loader));
				show_progress_dialog(loader);
			}
		}
		catch(Exception& e)
		{
			wxMessageBox(string_to_wxString(e.cause()), wxT("Error"), wxOK|wxICON_ERROR|wxCENTRE, this);
		}
	}
}

//#################### EVENT TABLE ####################
BEGIN_EVENT_TABLE(MainWindow, wxFrame)
	//~~~~~~~~~~~~~~~~~~~~ MENUS ~~~~~~~~~~~~~~~~~~~~
	EVT_MENU(MENUID_FILE_EXIT, MainWindow::OnMenuFileExit)
	EVT_MENU(MENUID_FILE_OPEN_DICOMDIR, MainWindow::OnMenuFileOpenDICOMDIR)
END_EVENT_TABLE()

}
