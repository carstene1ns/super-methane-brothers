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
#include "doc.h"

bool GLOBAL_SoundEnable = true;

SDL_Renderer *sdlRenderer;
SDL_Texture *sdlTexture;
SDL_Surface *sdlSurface;

std::shared_ptr<CMethDoc> m_Game;

void init_game() {
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_PNG);

	SDL_Window *sdlWindow = SDL_CreateWindow("Super Methane Brothers",
	                                         SDL_WINDOWPOS_UNDEFINED,
	                                         SDL_WINDOWPOS_UNDEFINED,
	                                         SCR_WIDTH * 2, SCR_HEIGHT * 2,
	                                         SDL_WINDOW_RESIZABLE);
	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	SDL_RenderSetLogicalSize(sdlRenderer, SCR_WIDTH, SCR_HEIGHT);
	SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
	sdlSurface = SDL_CreateRGBSurface(0, SCR_WIDTH, SCR_HEIGHT, 32,
	                                  0x00FF0000,
	                                  0x0000FF00,
	                                  0x000000FF,
	                                  0xFF000000);
	if(!sdlSurface) SDL_Log("Couldn't create surface: %s\n", SDL_GetError());
	sdlTexture = SDL_CreateTexture(sdlRenderer,
	                               SDL_PIXELFORMAT_ARGB8888,
	                               SDL_TEXTUREACCESS_STREAMING,
	                               SCR_WIDTH, SCR_HEIGHT);
	if(!sdlTexture) SDL_Log("Couldn't create texture: %s\n", SDL_GetError());

	m_Game = std::make_shared<CMethDoc>(sdlRenderer, sdlSurface, sdlTexture);

	m_Game->InitGame();
	m_Game->LoadScores();
	m_Game->StartGame();
}

void deinit_game() {
	m_Game->SaveScores();

	SDL_FreeSurface(sdlSurface);
	SDL_DestroyTexture(sdlTexture);
	SDL_DestroyRenderer(sdlRenderer);

	IMG_Quit();
	SDL_Quit();
}

void set_joy(SDL_Keycode sym, bool pressed) {
	// Get Controller state
	JOYSTICK *jptr1 = &m_Game->m_GameTarget.m_Joy1;
	JOYSTICK *jptr2 = &m_Game->m_GameTarget.m_Joy2;
	int val = pressed ? 1 : 0;

	// Player 1
	if (sym == SDLK_LEFT) jptr1->left = val;
	else if (sym == SDLK_RIGHT) jptr1->right = val;
	else if (sym == SDLK_UP) jptr1->up = val;
	else if (sym == SDLK_DOWN) jptr1->down = val;
	else if (sym == SDLK_SPACE) jptr1->fire = val;
	else if (sym == SDLK_TAB && pressed) {
		// Switch Player sprites
		m_Game->m_GameTarget.m_Game.TogglePuffBlow();
	}
	else if (sym == SDLK_F11) {
		// CHEAT MODE: warp levels
		jptr1->next_level = val;
	}

	// Player 2
	else if (sym == SDLK_a) jptr2->left = val;
	else if (sym == SDLK_d) jptr2->right = val;
	else if (sym == SDLK_w) jptr2->up = val;
	else if (sym == SDLK_s) jptr2->down = val;
	else if (sym == SDLK_LCTRL) jptr2->fire = val;

	// TODO: a-z, 0-9, space, enter

	//else SDL_Log("Unhandled key: %s\n", SDL_GetKeyName(sym));
}

bool run_game()
{
	SDL_Event event;

	// input loop
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				return false;
				break;
			case SDL_KEYDOWN:
				set_joy(event.key.keysym.sym, true);
				break;
			case SDL_KEYUP:
				set_joy(event.key.keysym.sym, false);
				break;
		}
	}
	m_Game->m_GameTarget.m_Joy1.key = 10;
#if 0
	jptr1->key = jptr2->key = ':';	// Fake key press (required for high score table)
#endif

	// clear canvas
	SDL_LockSurface(sdlSurface);
	SDL_memset(sdlSurface->pixels, 0, sdlSurface->h * sdlSurface->pitch);
	SDL_UnlockSurface(sdlSurface);

	m_Game->MainLoop();

	return true;
}

int main (int argc, char *argv[]) {
	(void) argc;
	(void) argv;

	init_game();
	int last_time = SDL_GetTicks();
	int game_speed = 60;

	while (run_game()) {
		int time_diff = 0;
		do {
			int time_now = SDL_GetTicks();
			time_diff = time_now - last_time;
			time_diff = game_speed - time_diff;
			if (time_diff > 50) SDL_Delay(20);
		} while(time_diff > 0);
		last_time = last_time + game_speed; 
	}

	deinit_game();

	return 0;
}
