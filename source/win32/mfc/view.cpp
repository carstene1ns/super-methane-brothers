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
// Methane Brothers View (Source File)
//------------------------------------------------------------------------------

#include <afx.h>
#include "methane.rc"
#include "Doc.h"
#include "View.h"
#include "global.h"
#include "methane.h"
#include "game.h"
#include "player.h"
#include "objtypes.h"

IMPLEMENT_DYNCREATE (CMethView, CView)

//------------------------------------------------------------------------------
// The message map
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CMethView, CView)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_KEYDOWN ()
	ON_WM_KEYUP ()

	ON_COMMAND_RANGE (ID_APP_ZOOM0, ID_APP_ZOOM4, OnAppZoom)
	ON_COMMAND (ID_APP_32COL, OnApp32Col)
	ON_COMMAND (ID_APP_RVIDEO, OnAppRVideo)
	ON_COMMAND (ID_APP_MONO, OnAppMono)
	ON_COMMAND (ID_APP_TOGGLE, OnAppToggle)

	ON_UPDATE_COMMAND_UI_RANGE (ID_APP_ZOOM0, ID_APP_ZOOM4, OnUpdateAppZoom)
	ON_UPDATE_COMMAND_UI (ID_APP_32COL, OnUpdateApp32Col)
	ON_UPDATE_COMMAND_UI (ID_APP_RVIDEO, OnUpdateAppRVideo)
	ON_UPDATE_COMMAND_UI (ID_APP_MONO, OnUpdateAppMono)
	ON_UPDATE_COMMAND_UI (ID_APP_TOGGLE, OnUpdateAppToggle)
END_MESSAGE_MAP()

//------------------------------------------------------------------------------
// Methview has been created.
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
CMethView::CMethView()
{
	m_32Col = m_RVideo = m_Mono = m_TogglePlayerGFX = m_ZCode = 0;
	m_pMemDC = NULL;
	m_pWinDC = NULL;
	m_pOldPalette = NULL;
	m_pOldBitmap = NULL;
	m_pBits = NULL;
	m_PaletteChangedFlag = 0;

}

//------------------------------------------------------------------------------
// View has been initially updated 
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::OnInitialUpdate()
{
	CView::OnInitialUpdate();			// Do standard update

	CMethDoc* pDoc = GetDocument ();		// Get the document
	m_rgbPalette = (RGBQUAD *) pDoc->m_GameTarget.m_rgbPalette;	// Steal the palette structure

	SetTimer(ID_TIMER_MAIN, 0, NULL);		// Set the program timer loop

	CreatePalette();				// Setup the colours
	CreateBitmap();					// Setup the bitmap

	pDoc->InitGame(this);				// Initialise the game
	pDoc->StartGame();				// Start the game

}

//------------------------------------------------------------------------------
// Destroy View
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::OnDestroy()
{
	CView::OnDestroy();

	KillTimer(ID_TIMER_MAIN);
	DeleteBitmap();
	DeletePalette();

}

//------------------------------------------------------------------------------
// Program Main Timer
// On Entry:
// 	nTimerID = Timer Resource ID
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::OnTimer( UINT nTimerID )
{
	if (!GetFocus()) return;
	CMethApp *app = (CMethApp *) AfxGetApp();
	if (app->m_DisableGame) return;
	app->m_SpeedTimer++;
	if (app->m_SpeedTimer <= app->m_GameSpeed) return;
	app->m_SpeedTimer = 0;

	CMethDoc* pDoc = GetDocument();
	pDoc->MainLoop();		  		// Do the program main loop
}

//------------------------------------------------------------------------------
// View to be ReDrawn 
// On Entry:
// 	pDC
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::OnDraw (CDC* pDC)
{
	UpdateScreen();
}

//------------------------------------------------------------------------------
// Update the current screen
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::UpdateScreen(void)
{
	if (m_PaletteChangedFlag)
	{
		PALETTEENTRY pe[PALETTE_SIZE];
		RGBQUAD quad[PALETTE_SIZE];

		PALETTEENTRY *pptr;
		PALETTEENTRY *pptr2;
		RGBQUAD *qptr;

		pptr = pe;
		int cnt;

		if (!m_Mono)
		{
			for (cnt=0; cnt<PALETTE_SIZE; cnt++, pptr++)
			{
				pptr->peRed = m_rgbPalette[cnt].rgbRed;
				pptr->peGreen = m_rgbPalette[cnt].rgbGreen;
				pptr->peBlue = m_rgbPalette[cnt].rgbBlue;
				pptr->peFlags = PC_RESERVED + PC_NOCOLLAPSE;
			}
		}else
		{
			int colourvalue;
			for (cnt=0; cnt<PALETTE_SIZE; cnt++, pptr++)
			{
				colourvalue = m_rgbPalette[cnt].rgbRed;
				colourvalue += m_rgbPalette[cnt].rgbGreen;
				colourvalue += m_rgbPalette[cnt].rgbBlue;

				colourvalue /= 3;

				pptr->peRed = colourvalue;
				pptr->peGreen = colourvalue;
				pptr->peBlue = colourvalue;
				pptr->peFlags = PC_RESERVED + PC_NOCOLLAPSE;
			}
		}
		if (m_32Col)
		{
			pptr = pe;
			pptr2 = &pe[PALETTE_SIZE/2];
			for (cnt=0; cnt<(PALETTE_SIZE/2); cnt++, pptr++, pptr2++)
			{
				pptr2->peRed = pptr->peRed;
				pptr2->peGreen = pptr->peGreen;
				pptr2->peBlue = pptr->peBlue;
			}
			
		}

		qptr = quad;
		pptr = pe;
		for (cnt=0; cnt<PALETTE_SIZE; cnt++, qptr++, pptr++)
		{
			qptr->rgbRed = pptr->peRed;
			qptr->rgbGreen = pptr->peGreen;
			qptr->rgbBlue = pptr->peBlue;
			qptr->rgbReserved = 0;
		}

		m_PaletteChangedFlag = 0;
		m_Palette.AnimatePalette(0,PALETTE_SIZE, pe);
		::SetDIBColorTable(m_pMemDC->m_hDC,0,PALETTE_SIZE,quad);
	}

	CRect myrect;
	GetWindowRect(&myrect);

	int copy_code;

	if (m_RVideo)
	{
		copy_code = NOTSRCCOPY;
	}else
	{
		copy_code = SRCCOPY;
	}

	// The line shown below does not do anything because of a windows 95 bug!
	//m_pWinDC->SetStretchBltMode(COLORONCOLOR);

	if (m_ZCode == ZCODE_NORMAL)
	{
		m_pWinDC->StretchBlt(0, 0, myrect.Width(), myrect.Height(), m_pMemDC, 0, 0, SCR_WIDTH, SCR_HEIGHT, copy_code);
	}

	if (m_ZCode >= ZCODE_ZOOM_1)
	{

		int xoff,yoff,width,height;
		int xorigin, yorigin;

		xorigin = SCR_WIDTH/2;
		yorigin = SCR_HEIGHT/2;

		CPlayerObj *play1;
		CMethDoc* pDoc = GetDocument();
		play1 = pDoc->m_GameTarget.m_Game.GetPlayer(OBJ_PLAYER_ONE);
		if (play1)
		{
			xorigin = play1->m_XPos + 16;
			yorigin = play1->m_YPos + 16;
		}else
		{
			xorigin = SCR_WIDTH/2;
			yorigin = SCR_HEIGHT/2;
		}

		// Calculate the graphic width to draw
		if (m_ZCode == ZCODE_ZOOM_1)
		{
			width = SCR_WIDTH-80;
			height = SCR_HEIGHT-64;
		}
		if (m_ZCode == ZCODE_ZOOM_2)
		{
			width = SCR_WIDTH-160;
			height = SCR_HEIGHT-128;
		}
		if (m_ZCode == ZCODE_ZOOM_3)
		{
			width = SCR_WIDTH-240;
			height = SCR_HEIGHT-196;
		}
	
		// Calculate the offset
		xoff = xorigin - (width/2);
		yoff = yorigin - (height/2);

		// Validate the offset
		if (xoff<0) xoff = 0;
		if (yoff<0) yoff = 0;
		if (xoff>(SCR_WIDTH - width)) xoff = SCR_WIDTH - width;
		if (yoff>(SCR_HEIGHT - height)) yoff = SCR_HEIGHT - height;

		m_pWinDC->StretchBlt(0, 0, myrect.Width(), myrect.Height(), m_pMemDC, xoff, yoff, width, height, copy_code);
	}
	if (m_ZCode == ZCODE_FAST)
	{
		int xoff, yoff;
		xoff = (myrect.Width()-SCR_WIDTH)/2;
		yoff = (myrect.Height()-SCR_HEIGHT)/2;
		m_pWinDC->BitBlt( xoff, yoff, SCR_WIDTH, SCR_HEIGHT, m_pMemDC, 0, 0, copy_code );
	}
}

//------------------------------------------------------------------------------
// Get the palette address - only if it is being used
// On Entry:
// 	Not Used
// On Exit:
// 	The palette
//		0 = Error
//------------------------------------------------------------------------------
CPalette* CMethView::GetPalette()
{
	if (m_Palette.m_hObject != NULL)
		return &m_Palette;
	else
		return NULL;
}

//------------------------------------------------------------------------------
// OnPaletteChanged
//------------------------------------------------------------------------------
void CMethView::OnPaletteChanged(CWnd* pFocusWnd) 
{
	if (pFocusWnd == this || IsChild(pFocusWnd))
		return;

	OnQueryNewPalette();
}

//------------------------------------------------------------------------------
// OnQueryNewPalette
//------------------------------------------------------------------------------
BOOL CMethView::OnQueryNewPalette() 
{
	if (GetPalette() == NULL)
		return FALSE;

	{
		CClientDC dc(this);
		CPalette* pOldPalette = dc.SelectPalette(&m_Palette,
			GetCurrentMessage()->message == WM_PALETTECHANGED);
		UINT nChanged = dc.RealizePalette();
		dc.SelectPalette(pOldPalette, TRUE);

		if (nChanged == 0)	// no change to our mapping
			return FALSE;
	}

	// some changes have been made; invalidate
	Invalidate(FALSE);

	return TRUE;
}

//------------------------------------------------------------------------------
// Delete the palette
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::DeletePalette(void)
{
	if (m_Palette.m_hObject != NULL)
		m_Palette.DeleteObject();
}

//------------------------------------------------------------------------------
// Create the palette
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::CreatePalette(void)
{
	LPLOGPALETTE lpLogPal;

	DeletePalette();

	lpLogPal = (LPLOGPALETTE) new BYTE[sizeof(LOGPALETTE) +
		((PALETTE_SIZE) * sizeof(PALETTEENTRY))];
	lpLogPal->palVersion = 0x0300;
	lpLogPal->palNumEntries = PALETTE_SIZE;

	for (int i = 0; i < PALETTE_SIZE; i++)
	{
		lpLogPal->palPalEntry[i].peRed = m_rgbPalette[i].rgbRed;
		lpLogPal->palPalEntry[i].peGreen = m_rgbPalette[i].rgbGreen;
		lpLogPal->palPalEntry[i].peBlue = m_rgbPalette[i].rgbBlue;
		lpLogPal->palPalEntry[i].peFlags = PC_RESERVED + PC_NOCOLLAPSE;
	}

	VERIFY( m_Palette.CreatePalette( lpLogPal ) );
	delete [] (BYTE *)lpLogPal;
}

//------------------------------------------------------------------------------
// Create the main screen bitmap
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::CreateBitmap(void)
{
	LPBITMAPINFO lpbi;

	DeleteBitmap();

	// Fill in the BITMAPINFOHEADER
	lpbi = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD))];
	lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbi->bmiHeader.biWidth = SCR_WIDTH;
	lpbi->bmiHeader.biHeight = -SCR_HEIGHT;
	lpbi->bmiHeader.biPlanes = 1;
	lpbi->bmiHeader.biBitCount = 8;
	lpbi->bmiHeader.biCompression = BI_RGB;
	lpbi->bmiHeader.biSizeImage = SCR_WIDTH * SCR_HEIGHT;
	lpbi->bmiHeader.biXPelsPerMeter = 0;
	lpbi->bmiHeader.biYPelsPerMeter = 0;
	lpbi->bmiHeader.biClrUsed = 0;
	lpbi->bmiHeader.biClrImportant = 0;

	// Fill in the color table
	memcpy( lpbi->bmiColors, m_rgbPalette, sizeof(RGBQUAD) * PALETTE_SIZE );

	m_pWinDC = new CWindowDC(this);

	HBITMAP hBitmap = CreateDIBSection( m_pWinDC->m_hDC, lpbi,
		DIB_RGB_COLORS, (void **)&m_pBits, NULL, 0 );

	delete [] (BYTE *)lpbi;

	ASSERT(hBitmap != NULL);

	m_Bitmap.Attach( hBitmap );

	m_pMemDC = new CDC;

	VERIFY( m_pMemDC->CreateCompatibleDC(m_pWinDC) );

	m_pOldPalette = m_pWinDC->SelectPalette( &m_Palette, FALSE );
	m_pWinDC->RealizePalette();

	m_pOldBitmap = m_pMemDC->SelectObject( &m_Bitmap );

	CWnd* pActiveWnd = CWnd::GetActiveWindow();
	if (pActiveWnd != NULL)
		pActiveWnd->SendMessage(WM_QUERYNEWPALETTE);

}

//------------------------------------------------------------------------------
// Delete the bitmap
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::DeleteBitmap(void)
{
	if (m_pMemDC != NULL)
	{
		m_pMemDC->SelectObject(m_pOldBitmap);
		delete m_pMemDC;
		m_pMemDC = NULL;
		m_pOldBitmap = NULL;
	}

	if (m_pWinDC != NULL)
	{
		m_pWinDC->SelectObject(m_pOldPalette);
		delete m_pWinDC;
		m_pWinDC = NULL;
		m_pOldPalette = NULL;
	}

	if (m_Bitmap.m_hObject != NULL)
		m_Bitmap.DeleteObject();

	m_pBits = NULL;
}

//------------------------------------------------------------------------------
// Process a keyboard stroke
// On Entry:
// 	nChar = key pressed
//		flag : 1 = down, 0 = up
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::KeyPress(UINT nChar, int flag)
{
	CMethDoc* pDoc = GetDocument ();		// Get the document
	switch (nChar)
	{
		case (VK_LEFT):
		{
			pDoc->m_GameTarget.m_Joy1.left = flag;
			break;
		}
		case (VK_RIGHT):
		{
			pDoc->m_GameTarget.m_Joy1.right = flag;
			break;
		}
		case (VK_UP):
		{
			pDoc->m_GameTarget.m_Joy1.up = flag;
			break;
		}
		case (VK_DOWN):
		{
			pDoc->m_GameTarget.m_Joy1.down = flag;
			break;
		}
		case (VK_CONTROL):
		{
			pDoc->m_GameTarget.m_Joy1.fire = flag;
			break;
		}

		case (VK_F1):
		{

			CMethApp *app = (CMethApp *) AfxGetApp();
			if (app->m_LevelSkipEnabled)
			{
				pDoc->m_GameTarget.m_Joy1.next_level = flag;
			}
			break;
		}

	}
}

//------------------------------------------------------------------------------
// Keyboard Up
// On Entry:
// 	nChar
//		nRepCnt
//		nFlags
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	KeyPress(nChar, 0);
}
//------------------------------------------------------------------------------
// Keyboard Down
// On Entry:
// 	nChar
//		nRepCnt
//		nFlags
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMethDoc* pDoc = GetDocument ();		// Get the document
 	pDoc->m_GameTarget.m_Joy1.key = nChar;
	KeyPress(nChar, 1);
}

//------------------------------------------------------------------------------
// Menu "Zoom"
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::OnAppZoom(UINT nID)
{
	m_ZCode = nID - ID_APP_ZOOM0;

	if (m_ZCode==ZCODE_FAST)
	{
		CWnd *win;
		win = GetParent();
		if (win)
		{
			win->MoveWindow(320,256, 320+8, 256+46);
			CenterWindow();
			//MoveWindow(0,0, 320, 256+64);
			//ModifyStyleEx(0, WS_EX_TRANSPARENT);
		}
	}

	Invalidate(TRUE);
}

//------------------------------------------------------------------------------
// Menu "32Colour mode"
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::OnApp32Col()
{
	m_32Col ^= 1;
	m_PaletteChangedFlag = 1;
}

//------------------------------------------------------------------------------
// Menu "Reverse Video"
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::OnAppRVideo()
{
	m_RVideo ^= 1;
}

//------------------------------------------------------------------------------
// Menu "Monochrome"
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::OnAppMono()
{
	m_Mono ^= 1;
	m_PaletteChangedFlag = 1;
}

//------------------------------------------------------------------------------
// Menu "Toggle" player graphic
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::OnAppToggle()
{
	m_TogglePlayerGFX ^= 1;

	CMethDoc* pDoc = GetDocument();
	pDoc->m_GameTarget.m_Game.TogglePuffBlow();
}


//------------------------------------------------------------------------------
// Menu "Zoom"
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::OnUpdateAppZoom(CCmdUI *pCmdUI)
{
	int code;

	code = pCmdUI->m_nID;

	code = code - ID_APP_ZOOM0;

	pCmdUI->SetCheck( code==m_ZCode );
}

//------------------------------------------------------------------------------
// Menu "32 colour mode"
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::OnUpdateApp32Col(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_32Col );
}

//------------------------------------------------------------------------------
// Menu "RVideo"
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::OnUpdateAppRVideo(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_RVideo );
}

//------------------------------------------------------------------------------
// Menu "Mono"
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::OnUpdateAppMono(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_Mono );
}


//------------------------------------------------------------------------------
// Menu "Toggle" player graphic
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethView::OnUpdateAppToggle(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_TogglePlayerGFX );
}



