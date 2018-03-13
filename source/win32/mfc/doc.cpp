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
// Initialise Document
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
CMethDoc::CMethDoc()
{
	m_Pause = 0;
	m_pMainView = 0;
#ifdef METHANE_MIKMOD
	SMB_NEW(m_pMikModDrv,CMikModDrv);
#endif
	m_GameTarget.Init(this);
}

//------------------------------------------------------------------------------
// Destroy Document
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
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
// New Document
// On Entry:
// 	Not Used
// On Exit:
// 	TRUE = Worked
//------------------------------------------------------------------------------
BOOL CMethDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

//------------------------------------------------------------------------------
// Close Document
// On Entry:
// 	Not Used
// On Exit:
// 	TRUE = Worked
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
// Open Document  
// On Entry:
// 	Not Used
// On Exit:
// 	TRUE = Worked
//------------------------------------------------------------------------------
BOOL CMethDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}

//------------------------------------------------------------------------------
// Save The Document data to disk
// On Entry:
// 	ar
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::Serialize(CArchive& ar)
{
}

//------------------------------------------------------------------------------
// Initialise the main game (Setup the pointers)
// On Entry:
// 	vptr = The Main View
// On Exit:
// 	Not Used
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
// Start the main game
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::StartGame(void)
{
	if (CheckValidGame())
	{
		m_GameTarget.StartGame();
	}
}

//------------------------------------------------------------------------------
// The Game Main Loop 
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
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
// Redraw the game main view
// On Entry:
// 	pal_change_flag : 0 = Palette not changed
// On Exit:
// 	Not Used
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
// Check to see if the game is valid - IE the major pointers are okay
// On Entry:
// 	Not Used
// On Exit:
// 	0 - Game is NOT valid
//		Else - Game is Okay
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
// Menu "Pause"
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::OnAppPause()
{
	m_Pause = m_Pause^1;
}

//------------------------------------------------------------------------------
// Menu restart the game
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::OnAppRestart()
{
	StartGame();
}

//------------------------------------------------------------------------------
// Menu "Sound off"
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
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
// Menu "Music off"
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
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
// Menu "Pause"
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::OnUpdateAppPause(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_Pause );
}

//------------------------------------------------------------------------------
// Menu "Sound off"
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
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
// Menu "Music off"
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
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
// Save the high scores
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
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
// Load the high scores
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
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
// Load the game options
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::LoadOptions(void)
{
	CMethApp *app = (CMethApp *) AfxGetApp();
	app->m_GameSpeed = app->GetProfileInt( txt_options, "Game Speed", 0 );
	app->m_LevelSkipEnabled = app->GetProfileInt( txt_options, "F1 Cheat", 0 );
}

//------------------------------------------------------------------------------
// Save the game options
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::SaveOptions(void)
{
	CMethApp *app = (CMethApp *) AfxGetApp();

	app->WriteProfileInt( txt_options, "Game Speed", app->m_GameSpeed );
	app->WriteProfileInt( txt_options, "F1 Cheat", app->m_LevelSkipEnabled );
}

//------------------------------------------------------------------------------
// Play a sample (called from the game)
// On Entry:
// 	id = SND_xxx id
//	pos = Sample Position to use 0 to 255
//	rate = The rate
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::PlaySample(int id, int pos, int rate)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->PlaySample(id, pos, rate);
#endif
}

//------------------------------------------------------------------------------
// Stop the module (called from the game)
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::StopModule(void)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->StopModule();
#endif
}

//------------------------------------------------------------------------------
// Play a module (called from the game)
// On Entry:
// 	id = SMOD_xxx id
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::PlayModule(int id)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->PlayModule(id);
#endif
}

//------------------------------------------------------------------------------
// Update the current module (ie restart the module if it has stopped) (called from the game)
// On Entry:
// 	id = SMOD_xxx id (The module that should be playing)
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::UpdateModule(int id)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->UpdateModule(id);
#endif
}

