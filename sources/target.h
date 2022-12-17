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

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include <ClanLib/sound.h>

#include "game.h"

class RenderBatchTriangle;
class CMethDoc;
class CGameTarget
{
private:
	CMethDoc *m_pDoc;	// The portal to the outside world

public:
	CGameTarget();
	void Init(CMethDoc *pdoc, clan::Canvas &canvas);
	void InitGame();
	void RedrawScreen();
	void StartGame();
	void MainLoop();
	void PrepareSoundDriver();
	void PlayModule(int id);
	void StopModule();
	void PlaySample(int id, int pos, int rate);
	void UpdateModule(int id);
	void Draw(int dest_xpos, int dest_ypos, int width, int height, int texture_number, int texture_xpos, int texture_ypos, bool draw_white);

	CGame m_Game;		// The Main Game
	JOYSTICK m_Joy1;	// To be written to by the OS
	JOYSTICK m_Joy2;	// To be written to by the OS
	int m_FadeChangeFlag;	// 0 = Palette has not changed

	float m_Lighting;	// -1 = Black. 0 = Normal. 1 = White

private:
	clan::Canvas m_Canvas;	//!< The canvas

	static const int m_NumTextures = 5;
	clan::Texture2D m_Texture[m_NumTextures];

	clan::SoundBuffer m_WAV_blow;
	clan::SoundBuffer m_WAV_bowling;
	clan::SoundBuffer m_WAV_candle;
	clan::SoundBuffer m_WAV_card;
	clan::SoundBuffer m_WAV_car;
	clan::SoundBuffer m_WAV_chicken;
	clan::SoundBuffer m_WAV_cookie;
	clan::SoundBuffer m_WAV_crying;
	clan::SoundBuffer m_WAV_day;
	clan::SoundBuffer m_WAV_die2;
	clan::SoundBuffer m_WAV_duck;
	clan::SoundBuffer m_WAV_feather;
	clan::SoundBuffer m_WAV_finlev1;
	clan::SoundBuffer m_WAV_hurry;
	clan::SoundBuffer m_WAV_marble;
	clan::SoundBuffer m_WAV_mask;
	clan::SoundBuffer m_WAV_moon;
	clan::SoundBuffer m_WAV_oil;
	clan::SoundBuffer m_WAV_pickup1;
	clan::SoundBuffer m_WAV_pstar;
	clan::SoundBuffer m_WAV_redstar;
	clan::SoundBuffer m_WAV_spiningtop;
	clan::SoundBuffer m_WAV_spit;
	clan::SoundBuffer m_WAV_splat;
	clan::SoundBuffer m_WAV_tap;
	clan::SoundBuffer m_WAV_train;
	clan::SoundBuffer m_WAV_tribble;
	clan::SoundBuffer m_WAV_turbo;
	clan::SoundBuffer m_WAV_twinkle;
	clan::SoundBuffer m_WAV_wings;
	clan::SoundBuffer m_WAV_wpotion;
	clan::SoundBuffer m_WAV_xylo;
	clan::SoundBuffer m_MOD_boss;
	clan::SoundBuffer m_MOD_complete;
	clan::SoundBuffer m_MOD_empty;
	clan::SoundBuffer m_MOD_title;
	clan::SoundBuffer m_MOD_tune1;
	clan::SoundBuffer m_MOD_tune2;

	clan::SoundBuffer_Session m_Session;
	bool m_bSessionActive;

	std::shared_ptr<RenderBatchTriangle> m_Batcher;

};

extern CGameTarget *GLOBAL_GameTarget;

extern bool GLOBAL_SoundEnable;

#endif


