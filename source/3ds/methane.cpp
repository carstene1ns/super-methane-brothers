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

/*
 * SDL2 Port by carstene1ns 07/2017, 12/2022
 */

//------------------------------------------------------------------------------
// Methane brothers main source file
//------------------------------------------------------------------------------
#include "precomp.h"
#include "global.h"

#include <memory>
#include <malloc.h>
#include "doc.h"

bool GLOBAL_SoundEnable = true;

std::shared_ptr<CMethDoc> m_Game;

#define SOC_ALIGN 0x1000
#define SOC_BUFFERSIZE 0x100000
static u32 *SOC_buffer = NULL;

static u32 old_time_limit;
static int game_speed = 60;

#define CLAMP(x, min, max) (x) <= (min) ? (min) : (x) >= (max) ? (max) : (x);

void init_game() {
	// allow audio thread to run on second core
	APT_GetAppCpuTimeLimit(&old_time_limit);
	APT_SetAppCpuTimeLimit(30);

	// enable 804 Mhz mode if on N3DS
	/*
	bool isN3DS;
	APT_CheckNew3DS(&isN3DS);
	if (isN3DS)
		osSetSpeedupEnable(true);
	*/
	gfxInitDefault();

	SOC_buffer = (u32*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);
	if(SOC_buffer == NULL)
		m_Game->m_GameTarget.ErrorOut("Failed to allocate SOC buffer");
	int ret = socInit(SOC_buffer, SOC_BUFFERSIZE);
	if (ret != 0)
    	m_Game->m_GameTarget.ErrorOut("Failed to start socket service");
	link3dsStdio();

	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	m_Game = std::make_shared<CMethDoc>();
	m_Game->InitGame();
	m_Game->LoadScores();
	m_Game->StartGame();
}

void deinit_game() {
	m_Game->SaveScores();
	m_Game->DeinitGame();

	C2D_Fini();
	C3D_Fini();

	socExit();

	gfxExit();

	if (old_time_limit != UINT32_MAX) {
		APT_SetAppCpuTimeLimit(old_time_limit);
	}
}

bool run_game() {
	bool want_exit = false;

	// check input, 1 player
	JOYSTICK *jptr = &m_Game->m_GameTarget.m_Joy1;
	hidScanInput();
	u32 input = hidKeysHeld();

	jptr->up = (input & KEY_DUP);
	jptr->down = (input & KEY_DDOWN);
	jptr->right = (input & KEY_DRIGHT);
	jptr->left = (input & KEY_DLEFT);
	jptr->fire = (input & KEY_A);

#if DEBUG
	// CHEAT MODE: warp levels
	jptr->next_level = (input & KEY_B);

	if (input & KEY_R) game_speed += 5;
	if (input & KEY_L) game_speed -= 5;
	game_speed = CLAMP(game_speed, 10, 110);
#endif

	input = hidKeysDown();
	// Switch Player sprites
	if (input & KEY_X) m_Game->m_GameTarget.m_Game.TogglePuffBlow();

	if(m_Game->m_GameTarget.m_Game.IsInGame()) {
		if (input & KEY_START)
			m_Game->m_GameTarget.m_Game.ReturnToTitle();
	} else {
		want_exit = (input & KEY_START);
	}	

	m_Game->MainLoop();

	return !want_exit;
}

int main (int argc, char *argv[]) {
	(void) argc;
	(void) argv;

	init_game();
	u64 last_time = osGetTime();

	while (aptMainLoop()) {
		if(!run_game()) break;
		int time_diff = 0;
		do {
			u64 time_now = osGetTime();
			time_diff = time_now - last_time;
			time_diff = game_speed - time_diff;
			if (time_diff > game_speed - 10) svcSleepThread(10 * 1000000ULL);
		} while(time_diff > 0);
		last_time = osGetTime();
	}

	deinit_game();

	return 0;
}
