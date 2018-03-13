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
// The "Help Dialog Box" (Header File)
//------------------------------------------------------------------------------

#ifndef _help_h
#define _help_h 1

#ifndef __AFXWIN_H__
#include <afxwin.h>
#endif

class CHelpDialog : public CDialog
{
private:
	CRect m_rect;

public:
	CHelpDialog (CWnd* pParentWnd = NULL) :
		CDialog (IDD_HELPDLG, pParentWnd) {}

	virtual BOOL OnInitDialog ();

protected:
	afx_msg void OnPaint ();
	DECLARE_MESSAGE_MAP ()
};

#endif


