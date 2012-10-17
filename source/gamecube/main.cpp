/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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

// Menu images
#include "menu_logo_bmp.h"
#include "menu_pointer_bmp.h"
#include "menu_music_bmp.h"
#include "menu_sound_bmp.h"
#include "menu_i_bmp.h"
#include "menu_widescreen_bmp.h"
#include "menu_yes_bmp.h"
#include "menu_no_bmp.h"
#include "menu_exit_bmp.h"
#include "menu_1player_bmp.h"
#include "menu_2players_bmp.h"
#include "menu_start_bmp.h"


// from SDL_cubevideo.c
extern "C" void CUBE_ChangeSquare(int xscale, int yscale, int xshift, int yshift);

// image loading helper function
SDL_Surface *load_menu_bitmap(const u8 *buffer, int filesize) 
{
	//Load the buffer into a surface using RWops
	SDL_RWops *rw = SDL_RWFromMem((void*) buffer, filesize);
	SDL_Surface *temp = SDL_LoadBMP_RW(rw, 1);

	//make it transparent
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, SDL_MapRGB(temp->format, 255, 0, 255)); 
 
	//Convert the image to optimal display format
	SDL_Surface *image;
	image = SDL_DisplayFormat(temp);
 
	//Free the temporary surface
	SDL_FreeSurface(temp);
 
	//Return our loaded image
	return image;
}

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

    // load Menu images
    SDL_Surface *menu_logo = load_menu_bitmap(menu_logo_bmp, menu_logo_bmp_size);
    SDL_Surface *menu_pointer = load_menu_bitmap(menu_pointer_bmp, menu_pointer_bmp_size);
    SDL_Surface *menu_1player = load_menu_bitmap(menu_1player_bmp, menu_1player_bmp_size);
    SDL_Surface *menu_2players = load_menu_bitmap(menu_2players_bmp, menu_2players_bmp_size);
    SDL_Surface *menu_start = load_menu_bitmap(menu_start_bmp, menu_start_bmp_size);
    SDL_Surface *menu_widescreen = load_menu_bitmap(menu_widescreen_bmp, menu_widescreen_bmp_size);
    SDL_Surface *menu_music = load_menu_bitmap(menu_music_bmp, menu_music_bmp_size);
    SDL_Surface *menu_sound = load_menu_bitmap(menu_sound_bmp, menu_sound_bmp_size);
    SDL_Surface *menu_i = load_menu_bitmap(menu_i_bmp, menu_i_bmp_size);
    SDL_Surface *menu_exit = load_menu_bitmap(menu_exit_bmp, menu_exit_bmp_size);
    SDL_Surface *menu_yes = load_menu_bitmap(menu_yes_bmp, menu_yes_bmp_size);
    SDL_Surface *menu_no = load_menu_bitmap(menu_no_bmp, menu_no_bmp_size);

    // Some globals
    int active_menu_item = 0; // Start
    int widescreen_enabled = 0; //nope
    int music_volume = 10; // max
    int sound_volume = 10; // max
    int i;
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

        /* DEBUG: alter game speed + warp levels
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

        // Add a delay
        int time_diff = 0;
        do
        {
            int time_now = SDL_GetTicks();
            time_diff = time_now - last_time;
            time_diff = game_speed - time_diff;
            if (time_diff > 50) SDL_Delay(20);
        } while(time_diff > 0);
        last_time = last_time + game_speed; 

        // Execute game logic
        Game.MainLoop(NULL, game_paused);

        // Display menu
        if (game_paused == 1)
        {
            // Draw logo + items
            SDL_Rect menu_pos;

            menu_pos.x = 16;
            menu_pos.y = 3;
            menu_pos.w = 0;
            menu_pos.h = 0;
            SDL_BlitSurface(menu_logo, NULL, screen, &menu_pos);

            menu_pos.x = 70;
            menu_pos.y = 120;
            menu_pos.w = 0;
            menu_pos.h = 0;
            SDL_BlitSurface(menu_start, NULL, screen, &menu_pos);

            menu_pos.x = 70;
            menu_pos.y = 140;
            menu_pos.w = 0;
            menu_pos.h = 0;

            if(players == 1)
                SDL_BlitSurface(menu_1player, NULL, screen, &menu_pos);
            else
                SDL_BlitSurface(menu_2players, NULL, screen, &menu_pos);

            menu_pos.x = 70;
            menu_pos.y = 160;
            menu_pos.w = 0;
            menu_pos.h = 0;
            SDL_BlitSurface(menu_widescreen, NULL, screen, &menu_pos);

            // Widescreen Indicator
            menu_pos.x = 255;
            menu_pos.y = 160;
            menu_pos.w = 0;
            menu_pos.h = 0;

            if(widescreen_enabled)
                SDL_BlitSurface(menu_yes, NULL, screen, &menu_pos);
            else
                SDL_BlitSurface(menu_no, NULL, screen, &menu_pos);

            menu_pos.x = 70;
            menu_pos.y = 180;
            menu_pos.w = 0;
            menu_pos.h = 0;
            SDL_BlitSurface(menu_sound, NULL, screen, &menu_pos);

            // Sound volume bar
            for (i = 1; i <= sound_volume; i++)
            {
                menu_pos.x = 160 + (i * 9);
                menu_pos.y = 180;
                menu_pos.w = 0;
                menu_pos.h = 0;
                SDL_BlitSurface(menu_i, NULL, screen, &menu_pos);
            }

            menu_pos.x = 70;
            menu_pos.y = 200;
            menu_pos.w = 0;
            menu_pos.h = 0;
            SDL_BlitSurface(menu_music, NULL, screen, &menu_pos);

            // Music volume bar
            for (i = 1; i <= music_volume; i++)
            {
                menu_pos.x = 160 + (i * 9);
                menu_pos.y = 200;
                menu_pos.w = 0;
                menu_pos.h = 0;
                SDL_BlitSurface(menu_i, NULL, screen, &menu_pos);
            }

            menu_pos.x = 70;
            menu_pos.y = 220;
            menu_pos.w = 0;
            menu_pos.h = 0;
            SDL_BlitSurface(menu_exit, NULL, screen, &menu_pos);

            // Pointer
            if((jptr1->down == 1) && (active_menu_item < 5))
                active_menu_item++;

            if((jptr1->up == 1) && (active_menu_item > 0))
                active_menu_item--;

            menu_pos.x = 30;
            menu_pos.y = 105 + (active_menu_item * 20);
            menu_pos.w = 0;
            menu_pos.h = 0;
            SDL_BlitSurface(menu_pointer, NULL, screen, &menu_pos);

            // React on items
            if((active_menu_item == 0) && (jptr1->fire)) // Start
            {
                game_paused = 0;
                Game.m_GameTarget.m_Game.InitGetPlayerNameScreen(players == 2);
            }

            if(active_menu_item == 1) // Players
            {
                if (jptr1->left)
                    players = 1;
                if (jptr1->right)
                    players = 2;
            }

            if(active_menu_item == 2) // Widescreen
            {
                if (jptr1->left)
                {
                    widescreen_enabled = 0;
                    CUBE_ChangeSquare(320, 240, 0, 0); // no padding
                }
                if (jptr1->right)
                {
                    widescreen_enabled = 1;
                    CUBE_ChangeSquare(280, 240, 0, 0); // add some padding
                }
            }

            if(active_menu_item == 3) // Sound
            {
                if((jptr1->right) && (sound_volume < 10))
                    sound_volume++;

                if((jptr1->left) && (sound_volume > 0))
                    sound_volume--;

                Game.ChangeVolume(sound_volume, music_volume);
            }

            if(active_menu_item == 4) // Music
            {
                if((jptr1->right) && (music_volume < 10))
                    music_volume++;

                if((jptr1->left) && (music_volume > 0))
                    music_volume--;

                Game.ChangeVolume(sound_volume, music_volume);
            }

            if((active_menu_item == 5) && (jptr1->fire)) //EXIT
                run = 0;

            // Show all
            SDL_Flip (screen);

            // Wait for buttonpresses
            SDL_Delay(60);

            // Fix delay timer
            last_time = SDL_GetTicks();
        }
    }

    Game.SaveScores();

    // save config here :)

    Game.RemoveSoundDriver();
    SDL_FreeSurface(screen);

    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    //return 0;
    exit(0);
}
