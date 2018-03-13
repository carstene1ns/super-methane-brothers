/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * Program WebSite: http://methane.sourceforge.net/index.html              *
 * Prgram Email: rombust@postmaster.co.uk                                  *
 *                                                                         *
 ***************************************************************************/

//------------------------------------------------------------------------------
// Methane Brothers Main Application (Source File)
//------------------------------------------------------------------------------

#include <afx.h>
#include "methane.rc"
#include "Methane.h"
#include "Frame.h"
#include "Doc.h"
#include "View.h"
#include "About.h"
#include "help.h"
#include "speed.h"

CMethApp myApp;	// This Main Application

//------------------------------------------------------------------------------
// Main Message Map
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CMethApp, CWinApp)
	ON_COMMAND (ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND (ID_APP_GAMESPEED, OnAppSpeed)
	ON_COMMAND (ID_APP_HELP, OnAppHelp)
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
//! \brief Program Started 
//!
//! 	\return TRUE = Worked
//------------------------------------------------------------------------------
BOOL CMethApp::InitInstance()
{

	m_DisableGame = 0;
	m_SpeedTimer = 0;
	m_LevelSkipEnabled = 0;

	SetRegistryKey ("Apache Software");
	LoadStdProfileSettings();

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate (
		IDR_MAINFRAME,
		RUNTIME_CLASS (CMethDoc),
		RUNTIME_CLASS (CMainFrame),
		RUNTIME_CLASS (CMethView)
	);

	AddDocTemplate (pDocTemplate);

	CCommandLineInfo cmdInfo;
	ParseCommandLine (cmdInfo);

	if (!ProcessShellCommand (cmdInfo))
		return FALSE;

	return TRUE;
}

//------------------------------------------------------------------------------
//! \brief Dialog "Help"
//------------------------------------------------------------------------------
void CMethApp::OnAppHelp()
{
	CHelpDialog dlg;
	m_DisableGame = 1;
	dlg.DoModal ();
	m_DisableGame = 0;
}

//------------------------------------------------------------------------------
//! \brief Dialog "Speed"
//------------------------------------------------------------------------------
void CMethApp::OnAppSpeed()
{
	CSpeedDialog dlg;
	m_DisableGame = 1;
	dlg.m_GameSpeed = m_GameSpeed;
	if (dlg.DoModal () == IDOK)
	{
		m_GameSpeed = dlg.m_GameSpeed;
	}
	m_DisableGame = 0;
}

//------------------------------------------------------------------------------
//! \brief Dialog "About"
//------------------------------------------------------------------------------
void CMethApp::OnAppAbout()
{
	CAboutDialog dlg;
	m_DisableGame = 1;
	dlg.DoModal ();
	m_DisableGame = 0;
}


