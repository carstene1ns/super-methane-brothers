/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * Program WebSite: http://methane.sourceforge.net/index.html              *
 *                                                                         *
 ***************************************************************************/

//------------------------------------------------------------------------------
// Methane Brothers Document (Header File)
//------------------------------------------------------------------------------

#ifndef _doc_h
#define _doc_h 1

#include "game.h"
#include "target.h"
#include "audiodrv.h"

class CMethDoc
{

public:
    CMethDoc();
    ~CMethDoc();

    CGameTarget m_GameTarget;

//GAME
    void InitGame(void);
    void StartGame(void);
    void MainLoop(void *screen_ptr, int paused_flag);

//GRAPHICS
    void RedrawMainView( int pal_change_flag );

//SOUND
    void InitSoundDriver(void);
    void RemoveSoundDriver(void);
    void PlayModule(int id);
    void StopModule(void);
    void UpdateModule(int id);
    void PlaySample(int id, int pos, int rate);

//HIGHSCORE
    void SaveScores(void);
    void LoadScores(void);

private:
    CAudioDrv *m_pAudioDrv;
    void DrawScreen( void *screen_ptr, int paused_flag );

};

#endif
