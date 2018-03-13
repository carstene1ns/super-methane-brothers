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
// Methane Brothers Document (Source File)
//------------------------------------------------------------------------------

#include <afxwin.h>
#include "methane.rc"
#include "methane.h"
#include "doc.h"
#include "view.h"
#include "target.h"

#ifdef METHANE_MIKMOD
#include "..\..\mikmod\audiodrv.h"
#endif

IMPLEMENT_DYNCREATE (CMethDoc, CDocument)

//------------------------------------------------------------------------------
// The message map
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CMethDoc, CDocument)
	ON_COMMAND (ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_UPDATE_COMMAND_UI (ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
	ON_COMMAND (ID_APP_PAUSE, OnAppPause)
	ON_COMMAND (ID_APP_RESTART, OnAppRestart)
	ON_COMMAND (ID_APP_MUSIC, OnAppMusicOff)
	ON_COMMAND (ID_APP_SOUND, OnAppSoundOff)
	ON_UPDATE_COMMAND_UI (ID_APP_PAUSE, OnUpdateAppPause)
	ON_UPDATE_COMMAND_UI (ID_APP_MUSIC, OnUpdateAppMusicOff)
	ON_UPDATE_COMMAND_UI (ID_APP_SOUND, OnUpdateAppSoundOff)

END_MESSAGE_MAP ()

static char txt_options[] = "Options";

//------------------------------------------------------------------------------
//! \brief Initialise Document
//------------------------------------------------------------------------------
CMethDoc::CMethDoc()
{
	m_Pause = 0;
	m_pMainView = 0;
#ifdef METHANE_MIKMOD
	m_pMikModDrv = new(CMikModDrv);
#endif
	m_GameTarget.Init(this);
}

//------------------------------------------------------------------------------
//! \brief Destroy Document
//------------------------------------------------------------------------------
CMethDoc::~CMethDoc()
{
#ifdef METHANE_MIKMOD
	if (m_pMikModDrv)
	{
		delete(m_pMikModDrv);
		m_pMikModDrv = 0;
	}
#endif
}

//------------------------------------------------------------------------------
//! \brief New Document
//!
//! 	\return TRUE = Worked
//------------------------------------------------------------------------------
BOOL CMethDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

//------------------------------------------------------------------------------
//! \brief Close Document
//!
//! 	\return TRUE = Worked
//------------------------------------------------------------------------------
void CMethDoc::OnCloseDocument()
{
	SaveOptions();
#ifdef METHANE_MIKMOD
	m_pMikModDrv->RemoveDriver();
#endif
	SaveScores();
	CDocument::OnCloseDocument();
}

//------------------------------------------------------------------------------
//! \brief Open Document  
//!
//! 	\return TRUE = Worked
//------------------------------------------------------------------------------
BOOL CMethDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}

//------------------------------------------------------------------------------
//! \brief Save The Document data to disk
//!
//! 	\param ar = ar
//------------------------------------------------------------------------------
void CMethDoc::Serialize(CArchive& ar)
{
}

//------------------------------------------------------------------------------
//! \brief Initialise the main game (Setup the pointers)
//!
//! 	\param vptr = The Main View
//------------------------------------------------------------------------------
void CMethDoc::InitGame(CMethView *vptr)
{
	m_pMainView = vptr;
	m_GameTarget.InitGame( (char *) vptr->m_pBits );
	m_GameTarget.PrepareSoundDriver();
	LoadOptions();
#ifdef METHANE_MIKMOD
	m_pMikModDrv->InitDriver();
#endif

	LoadScores();
}

//------------------------------------------------------------------------------
//! \brief Start the main game
//------------------------------------------------------------------------------
void CMethDoc::StartGame(void)
{
	if (CheckValidGame())
	{
		m_GameTarget.StartGame();
	}
}

//------------------------------------------------------------------------------
//! \brief The Game Main Loop 
//------------------------------------------------------------------------------
void CMethDoc::MainLoop( void )
{
	if (m_Pause) return;	// Pause mode

	if (CheckValidGame())
	{
		m_GameTarget.MainLoop();
	}
#ifdef METHANE_MIKMOD
	m_pMikModDrv->Update();
#endif
}

//------------------------------------------------------------------------------
//! \brief Redraw the game main view
//!
//! 	\param pal_change_flag = 0 = Palette not changed
//------------------------------------------------------------------------------
void CMethDoc::RedrawMainView( int pal_change_flag )
{
	if (m_pMainView)
	{
		m_pMainView->m_PaletteChangedFlag = pal_change_flag;
		m_pMainView->UpdateScreen();
	}
}

//------------------------------------------------------------------------------
//! \brief Check to see if the game is valid - IE the major pointers are okay
//!
//! 	\param 0 - Game is NOT valid. Else - Game is Okay
//------------------------------------------------------------------------------
int CMethDoc::CheckValidGame( void )
{
	if (m_pMainView)
	{
		if (m_pMainView->m_pBits)
		{
			return 1;
		}
	}
	return 0;
}

//------------------------------------------------------------------------------
//! \brief Menu "Pause"
//------------------------------------------------------------------------------
void CMethDoc::OnAppPause()
{
	m_Pause = m_Pause^1;
}

//------------------------------------------------------------------------------
//! \brief Menu restart the game
//------------------------------------------------------------------------------
void CMethDoc::OnAppRestart()
{
	StartGame();
}

//------------------------------------------------------------------------------
//! \brief Menu "Sound off"
//------------------------------------------------------------------------------
void CMethDoc::OnAppSoundOff()
{
#ifdef METHANE_MIKMOD
	if (m_pMikModDrv->m_DisableSamplesFlag)
	{
		m_pMikModDrv->m_DisableSamplesFlag = 0;
	}else	m_pMikModDrv->m_DisableSamplesFlag = 1;
#endif
}

//------------------------------------------------------------------------------
//! \brief Menu "Music off"
//------------------------------------------------------------------------------
void CMethDoc::OnAppMusicOff()
{
#ifdef METHANE_MIKMOD
	if (m_pMikModDrv->m_DisableMusicFlag)
	{
		m_pMikModDrv->m_DisableMusicFlag = 0;
		if (m_GameTarget.m_Game.m_pSound)
		{
			m_pMikModDrv->StopModule();
			m_GameTarget.m_Game.m_pSound->UpdateModule();
		}
	}else
	{
		m_pMikModDrv->m_DisableMusicFlag = 1;
		m_pMikModDrv->PlayModule(0);	// Since m_DisableMusicFlag is set music will stop
	}
#endif
}

//------------------------------------------------------------------------------
//! \brief Menu "Pause"
//------------------------------------------------------------------------------
void CMethDoc::OnUpdateAppPause(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_Pause );
}

//------------------------------------------------------------------------------
//! \brief Menu "Sound off"
//------------------------------------------------------------------------------
void CMethDoc::OnUpdateAppSoundOff(CCmdUI *pCmdUI)
{
#ifdef METHANE_MIKMOD
	pCmdUI->Enable(m_pMikModDrv->m_AudioValidFlag);
	pCmdUI->SetCheck( m_pMikModDrv->m_DisableSamplesFlag );
#else
	pCmdUI->Enable(0);
#endif
}

//------------------------------------------------------------------------------
//! \brief Menu "Music off"
//------------------------------------------------------------------------------
void CMethDoc::OnUpdateAppMusicOff(CCmdUI *pCmdUI)
{
#ifdef METHANE_MIKMOD
	pCmdUI->Enable(m_pMikModDrv->m_AudioValidFlag);
	pCmdUI->SetCheck( m_pMikModDrv->m_DisableMusicFlag );
#else
	pCmdUI->Enable(0);
#endif
}

//------------------------------------------------------------------------------
//! \brief Save the high scores
//------------------------------------------------------------------------------
void CMethDoc::SaveScores(void)
{
	CMethApp *app = (CMethApp *) AfxGetApp();
	char buffer[32];
	char name[32];
	HISCORES *hs;
	int cnt;
	for (cnt=0, hs=m_GameTarget.m_Game.m_HiScores; cnt<MAX_HISCORES; cnt++, hs++)
	{
		wsprintf( buffer, "High_%d", cnt+1 );
		memcpy( name, hs->name, 4 );
		wsprintf( &name[4], "%d", hs->score );

		app->WriteProfileString( "High Scores", buffer, name );

	}
}

//------------------------------------------------------------------------------
//! \brief Load the high scores
//------------------------------------------------------------------------------
void CMethDoc::LoadScores(void)
{
	CMethApp *app = (CMethApp *) AfxGetApp();
	char buffer[32];
	char *txt;

	for (int cnt=0; cnt<MAX_HISCORES; cnt++)
	{
		wsprintf( buffer, "High_%d", cnt+1 );
		CString str = app->GetProfileString( "High Scores", buffer );
		txt = str.GetBuffer(32);
		if (txt) if (txt[0])
		{
			m_GameTarget.m_Game.InsertHiScore( atoi(&txt[4]), txt );
		}
		str.ReleaseBuffer();

	}
}

//------------------------------------------------------------------------------
//! \brief Load the game options
//------------------------------------------------------------------------------
void CMethDoc::LoadOptions(void)
{
	CMethApp *app = (CMethApp *) AfxGetApp();
	app->m_GameSpeed = app->GetProfileInt( txt_options, "Game Speed", 0 );
	app->m_LevelSkipEnabled = app->GetProfileInt( txt_options, "F1 Cheat", 0 );
}

//------------------------------------------------------------------------------
//! \brief Save the game options
//------------------------------------------------------------------------------
void CMethDoc::SaveOptions(void)
{
	CMethApp *app = (CMethApp *) AfxGetApp();

	app->WriteProfileInt( txt_options, "Game Speed", app->m_GameSpeed );
	app->WriteProfileInt( txt_options, "F1 Cheat", app->m_LevelSkipEnabled );
}

//------------------------------------------------------------------------------
//! \brief Play a sample (called from the game)
//!
//! 	\param id = SND_xxx id
//!	\param pos = Sample Position to use 0 to 255
//!	\param rate = The rate
//------------------------------------------------------------------------------
void CMethDoc::PlaySample(int id, int pos, int rate)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->PlaySample(id, pos, rate);
#endif
}

//------------------------------------------------------------------------------
//! \brief Stop the module (called from the game)
//------------------------------------------------------------------------------
void CMethDoc::StopModule(void)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->StopModule();
#endif
}

//------------------------------------------------------------------------------
//! \brief Play a module (called from the game)
//!
//! 	\param id = SMOD_xxx id
//------------------------------------------------------------------------------
void CMethDoc::PlayModule(int id)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->PlayModule(id);
#endif
}

//------------------------------------------------------------------------------
//! \brief Update the current module (ie restart the module if it has stopped) (called from the game)
//!
//! 	\param id = SMOD_xxx id (The module that should be playing)
//------------------------------------------------------------------------------
void CMethDoc::UpdateModule(int id)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->UpdateModule(id);
#endif
}

