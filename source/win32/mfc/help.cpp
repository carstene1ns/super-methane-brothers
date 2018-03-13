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
// The "Help Dialog Box" (Source File)
//------------------------------------------------------------------------------

#include <afx.h>
#include "global.h"
#include "help.h"

//------------------------------------------------------------------------------
// The message map
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CHelpDialog, CDialog)
	ON_WM_PAINT ()
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
//! \brief Initialise the dialog 
//------------------------------------------------------------------------------
BOOL CHelpDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

//------------------------------------------------------------------------------
//! \brief Draw the Dialog 
//------------------------------------------------------------------------------
void CHelpDialog::OnPaint()
{
	CPaintDC dc (this);
	CPen *old_pen;
	CFont *old_font;
	CBrush *old_brush;

	CPen pen;
	CBrush brush;
	CFont font;

	pen.CreatePen( PS_SOLID, 2, RGB(0,0,0) );
	brush.CreateSolidBrush(RGB(255,255,255));
	old_brush = dc.SelectObject(&brush);
	old_pen = dc.SelectObject(&pen);
	dc.RoundRect(8,8,420,190,32,32);

	int oldmode = dc.SetBkMode(OPAQUE);

	font.CreateFont( 16, 0, 0, 0, FW_NORMAL, 0, 0, 0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FW_DONTCARE, "Times New Roman");

	old_font = dc.SelectObject(&font);

	CRect rec;
	rec.SetRect(24,24, 400, 250);
	dc.DrawText("Quick Instructions\n\n"
		"Press Fire to start. Use keyboard to enter player names.\n"
		"Press Return to start game. Tap Fire to fire gas from the gun.\n"
		"Hold Fire to suck a trapped baddie into the gun.\n"
		"Release Fire to throw the trapped baddie from the gun.\n"
		"Throw baddies at the wall to destroy them.\n\n"
		"Player One - Use Cursor keys and CTRL to fire.\n"
		"Player Two - Use A,W,S,D and shift to fire.\n",-1, &rec, 0);
		dc.SetBkMode(oldmode);

	if (old_font) dc.SelectObject(old_font);
	if (old_brush) dc.SelectObject(old_brush);
	if (old_pen) dc.SelectObject(old_pen);

}

