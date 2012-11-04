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
 * GameCube Port by infact <infact quantentunnel de> 10/2012
 */

//------------------------------------------------------------------------------
// Methane brothers main source file
//------------------------------------------------------------------------------

#include <gccore.h>
#include "SDL.h"
#include "global.h"
#include "doc.h"

//------------------------------------------------------------------------------
// The Game Instance
//------------------------------------------------------------------------------
CMethDoc Game;

int main (int argc, char **argv)
{
    // init SDL (Video only)
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Couldn't initialise SDL: %s", SDL_GetError());
        return 1;
    }
    atexit(SDL_Quit);

    screen = SDL_SetVideoMode(320, 240, 8, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
    if (!screen)
    {
        fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
        return 1;
    }

    SDL_ShowCursor(SDL_DISABLE);

    Game.InitSoundDriver();
    Game.InitGame();
    Game.LoadScores();
    Game.StartGame();

    int run = 1;
    int game_paused = 0;
    int game_speed = 55;
    int last_time = SDL_GetTicks();

    while(run)
    {
        // Get Controller state
        JOYSTICK *jptr1;
        JOYSTICK *jptr2;
        jptr1 = &Game.m_GameTarget.m_Joy1;
        jptr2 = &Game.m_GameTarget.m_Joy2;

        PAD_ScanPads();
        int bu_down, bu_up = 0;

        // Player 1
        bu_down = PAD_ButtonsDown(0);
        bu_up = PAD_ButtonsUp(0);

        // left
        if (PAD_StickX(0) <= -56)
            jptr1->left = 1;
        else
            jptr1->left = 0;

        // right
        if (PAD_StickX(0) >= 56)
            jptr1->right = 1;
        else
            jptr1->right = 0;

        // down
        if (PAD_StickY(0) <= -56)
            jptr1->down = 1;
        else
            jptr1->down = 0;

        // up
        if (PAD_StickY(0) >= 56)
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
        
        /* DEBUG: alter game speed + warp levels
        if (bu_down & PAD_TRIGGER_L) {
            game_speed += 5;
            if (game_speed > 120) game_speed = 120;
        }
        if (bu_down & PAD_TRIGGER_R) {
            game_speed -= 5;
            if (game_speed < 20) game_speed = 20;
        }
        if ((PAD_SubStickX(0) > 60) && (PAD_SubStickY(0) <= -60))
            jptr1->next_level = 1; else jptr1->next_level = 0;
        // DEBUG: end */

        // Player 2
        bu_down = PAD_ButtonsDown(1);
        bu_up = PAD_ButtonsUp(1);

        // left
        if (PAD_StickX(1) <= -56)
            jptr2->left = 1;
        else
            jptr2->left = 0;

        // right
        if (PAD_StickX(1) >= 56)
            jptr2->right = 1;
        else
            jptr2->right = 0;

        // down
        if (PAD_StickY(1) <= -56)
            jptr2->down = 1;
        else
            jptr2->down = 0;

        // up
        if (PAD_StickY(1) >= 56)
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

        int time_diff = 0;
        do
        {
            int time_now = SDL_GetTicks();
            time_diff = time_now - last_time;
            time_diff = game_speed - time_diff;
            if (time_diff > 50) SDL_Delay(20);
        } while(time_diff > 0);
        last_time = last_time + game_speed; 

        Game.MainLoop(NULL, game_paused);
    }

    Game.SaveScores();
    Game.RemoveSoundDriver();
    SDL_FreeSurface(screen);

    return 0;
}
