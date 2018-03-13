/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * Program WebSite: http://www.methane.fsnet.co.uk/index.html              *
 * Prgram Email: rombust@postmaster.co.uk                                  *
 *                                                                         *
 ***************************************************************************/

//------------------------------------------------------------------------------
// The "Speed Dialog Box" (Source File)
//------------------------------------------------------------------------------

#include <afx.h>
#include "global.h"
#include "speed.h"

//------------------------------------------------------------------------------
// The message map
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CSpeedDialog, CDialog)

END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// Initialise the dialog 
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
BOOL CSpeedDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

//------------------------------------------------------------------------------
// Exchange the data
// On Entry:
//		pDX
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CSpeedDialog::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SPEEDVALUE, m_GameSpeed);
	DDV_MinMaxInt(pDX, m_GameSpeed, 0, 20);
}

