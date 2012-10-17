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

#include "SDL.h"
#include "global.h"
#include "doc.h"

//------------------------------------------------------------------------------
// The Game Instance
//------------------------------------------------------------------------------
CMethDoc Game;

int main (int argc, char **argv)
{
    // init SDL
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

    while(run)
    {
        // just do something, so the cpu keeps calm
        SDL_Delay(60);

        Game.MainLoop(NULL, game_paused);
    }

    Game.SaveScores();
    Game.RemoveSoundDriver();
    SDL_FreeSurface(screen);

    return 0;
}
