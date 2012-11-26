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
 * GameCube/Wii Port by Infact <infact quantentunnel de> 2012
 */

//------------------------------------------------------------------------------
// Methane brothers main source file
//------------------------------------------------------------------------------

#include <gccore.h>
#include <ogcsys.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>

#include "global.h"
#include "doc.h"

// GC/Wii framebuffer and video mode
static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

//------------------------------------------------------------------------------
// The Game Instance
//------------------------------------------------------------------------------
CMethDoc Game;

int main (int argc, char **argv)
{
	// Stock init code
	VIDEO_Init();

	PAD_Init();

#ifdef HW_RVL
	WPAD_Init();
#endif /* HW_RVL */

	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);

	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();

	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

    Game.InitSoundDriver();
    Game.InitGame();
    Game.LoadScores();
    Game.StartGame();

    int run = 1;
    int game_paused = 0;
    int game_speed = 55;

    // Some globals
    int music_volume = 10; // max
    int sound_volume = 10; // max
    int players = 1;

    // load config here :)

    while(run)
    {
        // Get Controller state
        JOYSTICK *jptr1 = &Game.m_GameTarget.m_Joy1;
        JOYSTICK *jptr2 = &Game.m_GameTarget.m_Joy2;

        PAD_ScanPads();
        int bu_down, bu_up = 0;

        // Player 1
        bu_down = PAD_ButtonsDown(0);
        bu_up = PAD_ButtonsUp(0);

        // left
        if (PAD_StickX(0) <= -58)
            jptr1->left = 1;
        else
            jptr1->left = 0;

        // right
        if (PAD_StickX(0) >= 58)
            jptr1->right = 1;
        else
            jptr1->right = 0;

        // down
        if (PAD_StickY(0) <= -58)
            jptr1->down = 1;
        else
            jptr1->down = 0;

        // up
        if (PAD_StickY(0) >= 58)
            jptr1->up = 1;
        else
            jptr1->up = 0;

        // fire
        if (bu_down & PAD_BUTTON_A)
            jptr1->fire = 1;

        if (bu_up & PAD_BUTTON_A)
            jptr1->fire = 0;

        // Switch Player sprites
        if (bu_down & PAD_TRIGGER_Z)
            Game.m_GameTarget.m_Game.TogglePuffBlow();

        // Only Player 1 can Pause the Game
        if (bu_down & PAD_BUTTON_START)
            game_paused ^= 1;

        // DEBUG: alter game speed + warp levels
        if (bu_down & PAD_TRIGGER_L) {
            game_speed += 5;
            if (game_speed > 100)
                game_speed = 100;
        }
        if (bu_down & PAD_TRIGGER_R) {
            game_speed -= 5;
            if (game_speed < 10)
                game_speed = 10;
        }
        if ((PAD_SubStickX(0) > 60) && (PAD_SubStickY(0) <= -60))
            jptr1->next_level = 1;
        else
            jptr1->next_level = 0;

        // EXIT
        if (bu_down & PAD_TRIGGER_Z)
            exit(0);
        // DEBUG: end */

        // Player 2
        bu_down = PAD_ButtonsDown(1);
        bu_up = PAD_ButtonsUp(1);

        // left
        if (PAD_StickX(1) <= -58)
            jptr2->left = 1;
        else
            jptr2->left = 0;

        // right
        if (PAD_StickX(1) >= 58)
            jptr2->right = 1;
        else
            jptr2->right = 0;

        // down
        if (PAD_StickY(1) <= -58)
            jptr2->down = 1;
        else
            jptr2->down = 0;

        // up
        if (PAD_StickY(1) >= 58)
            jptr2->up = 1;
        else
            jptr2->up = 0;

        // fire
        if (bu_down & PAD_BUTTON_A)
            jptr2->fire = 1;

        if (bu_up & PAD_BUTTON_A)
            jptr2->fire = 0;

        // Fake a key press (to pass getPlayerName screen)
        jptr1->key = 13;

        // Execute game logic
        Game.MainLoop(NULL, game_paused);

        // wait for vsync
        VIDEO_WaitVSync();
    }

    Game.SaveScores();

    // save config here :)

    Game.RemoveSoundDriver();

    //return 0;
    exit(0);
}
