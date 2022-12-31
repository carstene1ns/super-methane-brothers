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
	void Init(CMethDoc *pdoc);
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
	void Draw(int dest_xpos, int dest_ypos, int sprite_number, bool draw_white = false, bool is_block = false);
	void ErrorOut(const char *message);
	void GetName(char *name);

	CGame m_Game;		// The Main Game
	JOYSTICK m_Joy1;	// To be written to by the OS
	JOYSTICK m_Joy2;	// To be written to by the OS
	int m_FadeChangeFlag;	// 0 = Palette has not changed

	float m_Lighting;	// -1 = Black. 0 = Normal. 1 = White

private:
	void DrawPlayerInfo();

	// video
	struct {
		C3D_Tex tex;
		C2D_Image image;
		C3D_RenderTarget *target;
		const Tex3DS_SubTexture subtex = {
			SCR_WIDTH, SCR_HEIGHT,
			0.0f, 1.0f, SCR_WIDTH/512.0f, 0.0f
		};
	} m_Canvas = {};
	C3D_RenderTarget *m_Top, *m_Bottom;
	C2D_SpriteSheet blocks, sprites;

	// audio
	MODULE *m_Musicfile[MODULE_COUNT-1];
	ndspWaveBuf m_Soundfile[SND_COUNT];
};

extern CGameTarget *GLOBAL_GameTarget;

extern bool GLOBAL_SoundEnable;

#define BLOCK_OFFSET_ENDBLOX 1591 // number of BLOX
#define BLOCK_OFFSET_TITLE 45 // number of ENDBLOX

#endif
