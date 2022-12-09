/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 * 3DS Port by carstene1ns 02/2017
 */

//------------------------------------------------------------------------------
// Methane brothers main source file
//------------------------------------------------------------------------------

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

#include <3ds.h>

#include "global.h"
#include "doc.h"

#define TICKS_PER_SEC (268123480)
#define TICKS_PER_VBL (TICKS_PER_SEC/60)

//------------------------------------------------------------------------------
// The Game Instance
//------------------------------------------------------------------------------
CMethDoc Game;

int main (int argc, char **argv)
{
	// init
	gfxInitDefault();
	consoleInit(GFX_BOTTOM, NULL);

	u32* gamebuf = (u32 *) linearAlloc(SCR_WIDTH * SCR_HEIGHT * sizeof(u32));

	Game.InitSoundDriver();
	Game.InitGame();
	Game.LoadScores();
	Game.StartGame();

	int game_paused = 0;
	int game_speed = 10;
	u64 last_time = svcGetSystemTick();

	// load config here :)

	while(aptMainLoop()) {
		//1 Player input loop
		JOYSTICK *jptr = &Game.m_GameTarget.m_Joy1;

		// Reset
		jptr->left = 0;
		jptr->right = 0;
		jptr->down = 0;
		jptr->up = 0;
		jptr->fire = 0;

		hidScanInput();

		int k_down = hidKeysDown();
		int k_held = hidKeysHeld();

		if (k_down & KEY_SELECT) break;
		if (k_held & KEY_DLEFT) jptr->left = 1;
		if (k_held & KEY_DRIGHT) jptr->right = 1;
		if (k_held & KEY_DDOWN) jptr->down = 1;
		if (k_held & KEY_DUP || k_held & KEY_A) jptr->up = 1;
		if (k_held & KEY_B) jptr->fire = 1;
		if (k_down & KEY_START) game_paused ^= 1;

		// Switch Player sprites
		if (k_down & KEY_X)	Game.m_GameTarget.m_Game.TogglePuffBlow();

#if 1 //def DEBUG
		// DEBUG: alter game speed + warp levels
		if (k_down & KEY_ZL && game_speed < 20) game_speed += 1;
		if (k_down & KEY_ZR && game_speed > 2) game_speed -= 1;

		if (k_down & KEY_CSTICK_LEFT && k_down & KEY_CSTICK_UP)
			jptr->next_level = 1;
		else
			jptr->next_level = 0;
#endif

		// Fake a key press (to pass getPlayerName screen)
		jptr->key = 13;

		// Add a delay
		u64 time_now = svcGetSystemTick();
		if (time_now < last_time + TICKS_PER_SEC/game_speed)
			svcSleepThread(last_time + TICKS_PER_SEC/game_speed - time_now);
		last_time = svcGetSystemTick();

		// Execute game logic
		Game.MainLoop(gamebuf, game_paused);

		// Draw to screen (rotated by 90 degrees)
		u8 *framebuf = (u8*) gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		for (int y = 3; y < SCR_HEIGHT - 13; y++)
			for (int x = 0; x < SCR_WIDTH; x++) {
				u32 pixel = gamebuf[y * SCR_WIDTH + x];
				u32 pos = 3 * (240 - (y - 2) + (x + 40) * 240);
				framebuf[pos] = (pixel >> 16) & 0xFF;
				framebuf[pos + 1] = (pixel >> 8) & 0xFF;
				framebuf[pos + 2] = (pixel >> 0) & 0xFF;
			}
		gfxFlushBuffers();
		gfxSwapBuffers();
	}

	Game.SaveScores();

	// save config here :)

	Game.RemoveSoundDriver();

	linearFree(gamebuf);

	// deinit
	gfxExit();

	return 0;
}
