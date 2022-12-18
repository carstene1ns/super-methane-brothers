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
// GameTarget class. Contains a wrapper to connect the game to the OS (Source File)
//------------------------------------------------------------------------------

#include <string>
#include <stdexcept>
#include <unistd.h>
#include "precomp.h"
#include "target.h"

#include "doc.h"

//------------------------------------------------------------------------------
// The game target (Yuck global!)
// Thus - Only a single GameTarget is allowed
//------------------------------------------------------------------------------
CGameTarget *GLOBAL_GameTarget = 0;

//------------------------------------------------------------------------------
//! \brief Constructor
//------------------------------------------------------------------------------
CGameTarget::CGameTarget()
{
	// Clear the joystick!
	memset(&m_Joy1, 0, sizeof(m_Joy1));
	memset(&m_Joy2, 0, sizeof(m_Joy2));

	m_FadeChangeFlag = 0;	// Palete has not changed

	GLOBAL_GameTarget = this;	// The global target

	m_Lighting = 0.0;
}

//------------------------------------------------------------------------------
//! \brief Initialisation
//!
//! 	\param pdoc = Pointer to the document this target belongs to
//!	\param canvas = Screen to draw to
//------------------------------------------------------------------------------
void CGameTarget::Init(CMethDoc *pdoc, SDL_Renderer* renderer, SDL_Surface* canvas, SDL_Texture* texture)
{
	m_Renderer = renderer;
	m_Canvas = canvas;
	m_Texture = texture;
	m_pDoc = pdoc;
}

//------------------------------------------------------------------------------
//! \brief Initialise the game
//------------------------------------------------------------------------------
void CGameTarget::InitGame()
{
	m_Game.Init(this, &m_Joy1, &m_Joy2);

	// Find the resources directory:
	std::string resource_dir = std::string("resources/");

	for(int i=0; i<5; i++) {
		std::string filename = resource_dir + "graphics/page_0" + std::to_string(i+1) + ".png";

		if (access(filename.c_str(), F_OK) != 0) {
			throw std::runtime_error("Unable to locate resources");
		}

		m_Spritesheet[i] = IMG_Load(filename.c_str());
		if(!m_Spritesheet[i]) {
			SDL_Log("Couldn't load %s: %s\n", filename.c_str(), SDL_GetError());
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Redraw screen (Called by the game)
//------------------------------------------------------------------------------
void CGameTarget::RedrawScreen()
{
	// clear
	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_Renderer);

	// draw game screen
	SDL_UpdateTexture(m_Texture, NULL, m_Canvas->pixels, m_Canvas->pitch);
	SDL_RenderCopy(m_Renderer, m_Texture, NULL, NULL);

	// fading
	if(m_Lighting != 0.0f) {
		Uint8 color, alpha;

		if(m_Lighting > 0.0f) {
			color = 255;
			alpha = 255 * m_Lighting;
		} else if(m_Lighting < 0.0f) {
			color = 0;
			alpha = 255 * m_Lighting * -1;
		}
		SDL_SetRenderDrawColor(m_Renderer, color, color, color, alpha);
		SDL_RenderFillRect(m_Renderer, NULL);
	}

	// show
	SDL_RenderPresent(m_Renderer);
}

//------------------------------------------------------------------------------
//! \brief Start the game
//------------------------------------------------------------------------------
void CGameTarget::StartGame()
{
	m_Game.StartGame();
}

//------------------------------------------------------------------------------
//! \brief Do the game main loop (Call every cycle)
//------------------------------------------------------------------------------
void CGameTarget::MainLoop()
{
	m_Game.MainLoop();
}

//------------------------------------------------------------------------------
//! \brief Prepare the sound driver (call before the game starts)
//------------------------------------------------------------------------------
void CGameTarget::PrepareSoundDriver()
{
	m_Game.m_Sound.PrepareAudio();
}

//------------------------------------------------------------------------------
//! \brief Play a module (called from the game)
//!
//! 	\param id = SMOD_xxx id
//------------------------------------------------------------------------------
void CGameTarget::PlayModule(int id)
{
	if (!GLOBAL_SoundEnable)
		return;
}

//------------------------------------------------------------------------------
//! \brief Stop the module (called from the game)
//------------------------------------------------------------------------------
void CGameTarget::StopModule()
{
	if (!GLOBAL_SoundEnable)
		return;
}

//------------------------------------------------------------------------------
//! \brief Play a sample (called from the game)
//!
//! 	\param id = SND_xxx id
//!	\param pos = Sample Position to use 0 to 255
//!	\param rate = The rate
//------------------------------------------------------------------------------
void CGameTarget::PlaySample(int id, int pos, int rate)
{
	if (!GLOBAL_SoundEnable)
		return;

	// Calculate panning from -1 to 1 (from 0 to 255)
	float pan = pos - 128;
	pan = pan / 128.0f;
	if (pan < -1.0f)
		pan = -1.0f;
	if (pan > 1.0f)
		pan = 1.0f;

}

//------------------------------------------------------------------------------
//! \brief Update the current module (ie restart the module if it has stopped)
//!
//! 	\param id = SMOD_xxx id (The module that should be playing)
//------------------------------------------------------------------------------
void CGameTarget::UpdateModule(int id)
{
	if (!GLOBAL_SoundEnable)
		return;
}

//------------------------------------------------------------------------------
//! \brief Draw
//! 
//!	\param 
//! 
//!	\return 
//------------------------------------------------------------------------------
void CGameTarget::Draw(int dest_xpos, int dest_ypos, int width, int height, int spritesheet_number, int spritesheet_xpos, int spritesheet_ypos, bool draw_white)
{
	const SDL_Rect srcrect = { spritesheet_xpos, spritesheet_ypos, width, height };
	SDL_Rect dstrect = { dest_xpos, dest_ypos, 0, 0 };

	if(draw_white) {
		SDL_Log("No idea how to draw white\n");
	}

	if (SDL_BlitSurface(m_Spritesheet[spritesheet_number], &srcrect, m_Canvas, &dstrect) != 0) {
		SDL_Log("Couldn't blit: %s\n", SDL_GetError());
	}
}
