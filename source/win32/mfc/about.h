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
// The "About Dialog Box" (Header File)
//------------------------------------------------------------------------------

#ifndef _about_h
#define _about_h 1

#ifndef __AFXWIN_H__
#include <afxwin.h>
#endif

class CAboutDialog : public CDialog
{
private:
	CRect m_rect;

public:
	CAboutDialog (CWnd* pParentWnd = NULL) :
		CDialog (IDD_ABOUTDLG, pParentWnd) {}

	virtual BOOL OnInitDialog ();

protected:
	afx_msg void OnPaint ();
	DECLARE_MESSAGE_MAP ()
};

#endif


