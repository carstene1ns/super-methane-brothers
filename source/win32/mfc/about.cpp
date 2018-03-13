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
// The "About Dialog Box" (Source File)
//------------------------------------------------------------------------------

#include <afx.h>
#include "global.h"
#include "About.h"

//------------------------------------------------------------------------------
// The message map
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CAboutDialog, CDialog)
	ON_WM_PAINT ()
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
//! \brief Initialise the dialog 
//------------------------------------------------------------------------------
BOOL CAboutDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CStatic* pStatic = (CStatic*) GetDlgItem(IDC_ICONRECT);
	pStatic->GetWindowRect(&m_rect);
	pStatic->DestroyWindow();
	ScreenToClient(&m_rect);		// Get where to put the large icon

	return TRUE;
}

//------------------------------------------------------------------------------
//! \brief Draw the Dialog 
//------------------------------------------------------------------------------
void CAboutDialog::OnPaint()
{
	CPaintDC dc (this);
	HICON hIcon = (HICON) ::GetClassLong(AfxGetMainWnd()->m_hWnd,
		GCL_HICON);

	if (hIcon != NULL)
	{
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);

		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(&dc, 32, 32);
		CBitmap* pOldBitmap = dcMem.SelectObject (&bitmap);

		CBrush brush(::GetSysColor(COLOR_3DFACE));
		dcMem.FillRect(CRect (0, 0, 32, 32), &brush);
		dcMem.DrawIcon(0, 0, hIcon);

		dc.StretchBlt(m_rect.left, m_rect.top, m_rect.Width(),
			m_rect.Height(), &dcMem, 0, 0, 32, 32, SRCCOPY);

		dcMem.SelectObject(pOldBitmap);
	}
}

