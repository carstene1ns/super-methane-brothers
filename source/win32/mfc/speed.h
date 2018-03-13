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
// The "Speed Dialog Box" (Header File)
//------------------------------------------------------------------------------

#ifndef _speed_h
#define _speed_h 1

#ifndef __AFXWIN_H__
#include <afxwin.h>
#endif

class CSpeedDialog : public CDialog
{

public:
	CSpeedDialog (CWnd* pParentWnd = NULL) :
		CDialog (IDD_GAMESPEED, pParentWnd) {}

	virtual BOOL OnInitDialog ();

protected:
	virtual void DoDataExchange(CDataExchange *);
	DECLARE_MESSAGE_MAP ()
public:
		int	m_GameSpeed;

};

#endif


