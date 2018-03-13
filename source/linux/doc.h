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

#include "../game.h"
#include "../target.h"

#ifdef METHANE_MIKMOD
class CMikModDrv;
#endif
class CMethDoc
{

public:
	CMethDoc();
	~CMethDoc();

	void InitGame(void);
	void StartGame(void);
	void MainLoop(void *screen_ptr, int paused_flag);
	void RedrawMainView( int pal_change_flag );
	void PlayModule(int id);
	void StopModule(void);
	void PlaySample(int id, int pos, int rate);
	void RemoveSoundDriver(void);
	void InitSoundDriver(void);
	void UpdateModule(int id);
	void DisplayOptions( void *screen_ptr, int page_no );
	void DrawFont32Text( int *dptr, int ypos, char *tptr);

	void SaveScores(void);
	void LoadScores(void);

private:
	void DrawScreen( void *screen_ptr, int paused_flag );
private:
#ifdef METHANE_MIKMOD
	CMikModDrv	*m_pMikModDrv;
#endif
public:
	CGameTarget	m_GameTarget;

};

#endif

