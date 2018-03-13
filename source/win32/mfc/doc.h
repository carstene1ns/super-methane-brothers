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
// Methane Brothers Document (Header File)
//------------------------------------------------------------------------------

#ifndef _doc_h
#define _doc_h 1

#ifndef __AFXWIN_H__
#include <afxwin.h>
#endif

#include "sound.h"
#include "game.h"
#include "target.h"

class CMethView;
#ifdef METHANE_MIKMOD
class CMikModDrv;
#endif
class CMethDoc : public CDocument
{
	DECLARE_DYNCREATE (CMethDoc)

private:
	int CheckValidGame( void );
	CMethDoc();
	~CMethDoc();

public:
	virtual BOOL OnNewDocument ();
	virtual void OnCloseDocument ();
	virtual BOOL OnOpenDocument (LPCTSTR);
	virtual void Serialize (CArchive&);

	void InitGame(CMethView *vptr);
	void StartGame(void);
	void MainLoop(void);
	void RedrawMainView( int pal_change_flag );
	void LoadScores(void);
	void SaveScores(void);
	void LoadOptions(void);
	void SaveOptions(void);
	void PlayModule(int id);
	void StopModule(void);
	void PlaySample(int id, int pos, int rate);
	void UpdateModule(int id);

public:
	int		m_Pause;

	CGameTarget	m_GameTarget;

private:
	CMethView	*m_pMainView;
#ifdef METHANE_MIKMOD
	CMikModDrv	*m_pMikModDrv;
#endif
protected:

	afx_msg void OnAppPause ();
	afx_msg void OnAppRestart ();
	afx_msg void OnAppMusicOff ();
	afx_msg void OnAppSoundOff ();
	afx_msg void OnAppAudioOff ();
	afx_msg void OnAppAudioOn ();

	afx_msg void OnUpdateAppPause (CCmdUI *);
	afx_msg void OnUpdateAppMusicOff (CCmdUI *);
	afx_msg void OnUpdateAppSoundOff (CCmdUI *);
	afx_msg void OnUpdateAppAudioOff (CCmdUI *);
	afx_msg void OnUpdateAppAudioOn (CCmdUI *);

	DECLARE_MESSAGE_MAP ()
};

#endif


