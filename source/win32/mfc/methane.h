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
// Methane Brothers Main Application (Header File)
//------------------------------------------------------------------------------

#ifndef _methane_h
#define _methane_h 1

#ifndef __AFXWIN_H__
#include <afxwin.h>
#endif

class CMethView;

class CMethApp : public CWinApp
{
public:
	virtual BOOL InitInstance ();
protected:
	afx_msg void OnAppAbout ();
	afx_msg void OnAppHelp ();
	afx_msg void OnAppSpeed ();
	DECLARE_MESSAGE_MAP ()
public:
	int	m_DisableGame;
	int m_GameSpeed;
	int m_SpeedTimer;
	int m_LevelSkipEnabled;
};

#endif

