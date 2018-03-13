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
// Methane Brothers Frame Window (Source File)
//------------------------------------------------------------------------------

#include <afxwin.h>
#include "methane.rc"
#include "view.h"
#include "Frame.h"

IMPLEMENT_DYNCREATE (CMainFrame, CFrameWnd)

//------------------------------------------------------------------------------
// The message map
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CMainFrame, CFrameWnd)
	ON_WM_CREATE ()
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
//! \brief Frame Window Created 
//!
//! 	lpcs = lpcs
//!
//! 	\return 0 = Worked
//------------------------------------------------------------------------------
int CMainFrame::OnCreate (LPCREATESTRUCT lpcs)
{
	int cols;
	if (CFrameWnd::OnCreate (lpcs) == -1)
		return -1;

#ifdef METHANE_MIKMOD
	MessageBox(	"Super Methane Brothers is about to start.\n\n"
				"The GNU General Public License V2 applies to this game.\n\n"
				"For the game instructions, use Help/Quick-Instructions on the menu bar.\n\n"
				"Web Page: http://methane.sourceforge.net\n\n"
				"NOTE: On NT Machines, increase \"Game Speed\" from 0 to 2 if the game is too fast\n\n"
				,"READ THIS NOW!!!" );
#else

	MessageBox(	"Super Methane Brothers is about to start.\n\n"
				"The GNU General Public License V2 applies to this game.\n\n"
				"Sound has been disabled from this version of the game.\n\n"
				"For the game instructions, use Help/Quick-Instructions on the menu bar.\n\n"
				"Web Page: http://methane.sourceforge.net\n\n"
				"NOTE: On NT Machines, increase \"Game Speed\" from 0 to 2 if the game is too fast\n\n"
				,"READ THIS NOW!!!" );
#endif
	CClientDC dc(this);

	cols = dc.GetDeviceCaps(NUMCOLORS);
	//char txt[64];
	//sprintf(txt, "Number of system colours = %d", cols);
	//MessageBox(txt);

	if (cols==16)
	{
		MessageBox("16 Colour Mode Detected. Please switch to 256 colour mode.");
	}

	if (cols>16)	// 256 colour mode
	{
		//MessageBox("256 Colour mode");
		// Test for non paletteable 256 colour
		if (!(dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE))
		{
			MessageBox("Your video card does not support 256 colour palette animation."
				"Please switch to a video mode with more colours."
				"Else you may not see graphics.");
		}
	}

	int caps;

	caps = dc.GetDeviceCaps(RASTERCAPS);
	if (!(caps&RC_BITBLT))
	{
		MessageBox("You video card will not copy bitmaps. Try a different video mode.");
	}else
	if (!(caps&RC_BITMAP64))
	{
		MessageBox("You video card will not support bitmaps greater than 64K. Try a different video mode.");
	}else
	if (!(caps&RC_STRETCHBLT))
	{
		MessageBox("You video card will not stretch bitmaps. Try a different video mode.");
	}
	return 0;
}

//------------------------------------------------------------------------------
//! \brief Remove this flag to remove " - Untitled" from the frame's caption
//!	\param cs = cs
//!	\return bool
//------------------------------------------------------------------------------
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style &= ~ FWS_ADDTOTITLE;
	cs.cx = 320+160;
	cs.cy = 256+128;

	return CFrameWnd::PreCreateWindow(cs);
}

