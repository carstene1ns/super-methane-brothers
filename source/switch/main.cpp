/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 * Nintendo Switch Port by carstene1ns 04/2018
 */

//------------------------------------------------------------------------------
// Methane brothers main source file
//------------------------------------------------------------------------------

#include <switch.h>

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <malloc.h>
#include <cmath>

#include "global.h"
#include "doc.h"

//------------------------------------------------------------------------------
// The Game Instance
//------------------------------------------------------------------------------
CMethDoc Game;

int main (int argc, char *argv[]) {
	(void) argc;
	(void) argv;

	// init
	gfxInitDefault();
	gfxSetMode(GfxMode_TiledDouble);
	// wide screen, keep aspect ratio
	gfxConfigureResolution(SCR_WIDTH + 136, SCR_HEIGHT);

	u32* gamebuf = (u32*) malloc(SCR_HEIGHT * SCR_WIDTH * sizeof(u32));

	Game.InitSoundDriver();
	Game.InitGame();
	Game.LoadScores();
	Game.StartGame();

	int game_paused = 0;

	while(appletMainLoop()) {
		hidScanInput();

		//1 Player input loop
		// Get Controller state
		JOYSTICK *jptr = &Game.m_GameTarget.m_Joy1;

		u32 keys_down = hidKeysDown(CONTROLLER_P1_AUTO);
		u32 keys_up = hidKeysUp(CONTROLLER_P1_AUTO);

		if (keys_down & KEY_PLUS) break;

		if (keys_down & KEY_LEFT) jptr->left = 1;
		if (keys_down & KEY_RIGHT) jptr->right = 1;
		if (keys_down & KEY_A) jptr->up = 1;
		if (keys_down & KEY_DOWN) jptr->down = 1;
		if (keys_down & KEY_B) jptr->fire = 1;

		if (keys_down & KEY_X) Game.m_GameTarget.m_Game.TogglePuffBlow();
		if (keys_down & KEY_MINUS) game_paused ^= 1;

		if (keys_up & KEY_LEFT) jptr->left = 0;
		if (keys_up & KEY_RIGHT) jptr->right = 0;
		if (keys_up & KEY_A) jptr->up = 0;
		if (keys_up & KEY_DOWN) jptr->down = 0;
		if (keys_up & KEY_B) jptr->fire = 0;

		// Fake a key press (to pass getPlayerName screen)
		jptr->key = 13;

		// Execute game logic
		Game.MainLoop(gamebuf, game_paused);

		// Draw to screen, upscaling in hardware
		u32 *framebuf = (u32*) gfxGetFramebuffer(NULL, NULL);
		for (int y = 0; y < SCR_HEIGHT; y++)
			for (int x = 0; x < SCR_WIDTH; x++)
				framebuf[gfxGetFramebufferDisplayOffset(x + 68, y)] = gamebuf[y * SCR_WIDTH + x];

		gfxFlushBuffers();
		gfxSwapBuffers();
		gfxWaitForVsync();

		// Add a delay, FIXME: calculate this with:
		//u64 time_now = svcGetSystemTick();
		svcSleepThread(40000000);
	}

	Game.SaveScores();

	// save config here :)

	Game.RemoveSoundDriver();

	// deinit
	free(gamebuf);
	gfxExit();

	return 0;
}
