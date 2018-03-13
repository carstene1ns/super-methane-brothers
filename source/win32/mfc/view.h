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
// Methane Brothers View (Header File)
//------------------------------------------------------------------------------

#ifndef _view_h
#define _view_h 1

//------------------------------------------------------------------------------
// Zoom codes
//------------------------------------------------------------------------------
#define ZCODE_NORMAL	0
#define ZCODE_FAST		1
#define ZCODE_ZOOM_1	2
#define ZCODE_ZOOM_2	3
#define ZCODE_ZOOM_3	4

class CMethDoc;

class CMethView : public CView
{
	DECLARE_DYNCREATE (CMethView)

private:
	CMethDoc* GetDocument () { return (CMethDoc*) m_pDocument; }
	CPalette* GetPalette();

	// Device Context Attributes
	CDC* m_pMemDC;
	CWindowDC* m_pWinDC;

	// Palette Attributes
	CPalette m_Palette;
	CPalette* m_pOldPalette;

	// Bitmap Attributes
	CBitmap m_Bitmap;
	CBitmap* m_pOldBitmap;

public:
	CMethView();
	virtual void OnInitialUpdate();

	void UpdateScreen(void);
	void DeleteBitmap(void);
	void CreateBitmap(void);
	void CreatePalette(void);
	void DeletePalette(void);

	int		m_ZCode;	// ZCODE_xxx codes
	BYTE* m_pBits;
	int m_PaletteChangedFlag;
	RGBQUAD *m_rgbPalette;	// Points to game palette of size PALETTE_SIZE
	int		m_32Col;
	int		m_RVideo;
	int		m_Mono;
	int		m_TogglePlayerGFX;

private:
	void KeyPress(UINT nChar, int flag);

protected:
	virtual void OnDraw (CDC*);

	afx_msg void OnTimer( UINT nTimerID );
	afx_msg void OnDestroy();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnKeyDown (UINT,UINT,UINT);
	afx_msg void OnKeyUp (UINT,UINT,UINT);
	afx_msg void OnAppZoom (UINT nID);
	afx_msg void OnApp32Col ();
	afx_msg void OnAppRVideo ();
	afx_msg void OnAppMono ();
	afx_msg void OnAppToggle ();

	afx_msg void OnUpdateAppZoom (CCmdUI *);
	afx_msg void OnUpdateApp32Col (CCmdUI *);
	afx_msg void OnUpdateAppRVideo (CCmdUI *);
	afx_msg void OnUpdateAppMono (CCmdUI *);
	afx_msg void OnUpdateAppToggle (CCmdUI *);
		
	DECLARE_MESSAGE_MAP ()
};

#endif

