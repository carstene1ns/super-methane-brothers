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
// GameTarget class Header File
//------------------------------------------------------------------------------

#ifndef _target_h
#define _target_h 1

#include "precomp.h"

#include "game.h"

class CMethDoc;
class CGameTarget
{
private:
	CMethDoc *m_pDoc;	// The portal to the outside world

public:
	CGameTarget();
	void Init(CMethDoc *pdoc, SDL_Renderer *renderer, SDL_Surface *canvas, SDL_Texture *texture);
	void InitGame();
	void DeinitGame();
	void RedrawScreen();
	void StartGame();
	void MainLoop();
	void PrepareSoundDriver();
	void FreeSoundDriver();
	void PlayModule(int id);
	void StopModule();
	void PlaySample(int id, int pos, int rate);
	void UpdateModule(int id);
	void Draw(int dest_xpos, int dest_ypos, int width, int height, int spritesheet_number, int spritesheet_xpos, int spritesheet_ypos, bool draw_white);

	CGame m_Game;		// The Main Game
	JOYSTICK m_Joy1;	// To be written to by the OS
	JOYSTICK m_Joy2;	// To be written to by the OS
	int m_FadeChangeFlag;	// 0 = Palette has not changed

	float m_Lighting;	// -1 = Black. 0 = Normal. 1 = White

private:
	SDL_Renderer *m_Renderer;
	SDL_Surface *m_Canvas;	//!< The canvas
	SDL_Texture *m_Texture;

	static const int m_NumSpritesheets = 5;
	SDL_Surface *m_Spritesheet[m_NumSpritesheets];

	SAMPLE *m_Soundfile[SND_COUNT];
	MODULE *m_Musicfile[MODULE_COUNT-1];
};

extern CGameTarget *GLOBAL_GameTarget;

extern bool GLOBAL_SoundEnable;

#endif
