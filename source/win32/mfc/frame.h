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
// Methane Brothers Frame Window (Header File)
//------------------------------------------------------------------------------

#ifndef _frame_h
#define _frame_h 1

#ifndef __AFXWIN_H__
#include <afxwin.h>
#endif

class CMainFrame : public CFrameWnd
{
	DECLARE_DYNCREATE (CMainFrame)

public:

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg int OnCreate (LPCREATESTRUCT);

	DECLARE_MESSAGE_MAP ()
};

#endif
