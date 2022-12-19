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

void sdl_audiocallback(void *, Uint8 *stream, int len) {
	VC_WriteBytes((SBYTE *)stream, len);
}

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
	if (access(resource_dir.c_str(), F_OK) != 0) {
		throw std::runtime_error("Unable to locate resources");
	}

	for(int i = 0; i < 5; i++) {
		std::string filename = resource_dir + "graphics/page_0" + std::to_string(i+1) + ".png";
		m_Spritesheet[i] = IMG_Load(filename.c_str());
		if(!m_Spritesheet[i]) {
			SDL_Log("Couldn't load %s: %s\n", filename.c_str(), SDL_GetError());
		}
	}
}

void CGameTarget::DeinitGame()
{
	for(int i = 0; i < 5; i++) {
		SDL_FreeSurface(m_Spritesheet[i]);
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
	std::string resource_dir = std::string("resources/");

	if (GLOBAL_SoundEnable)	{
		MikMod_InitThreads();
		MikMod_RegisterDriver(&drv_nos);
		MikMod_RegisterAllLoaders();

		md_mode |= DMODE_SOFT_MUSIC | DMODE_SOFT_SNDFX | DMODE_NOISEREDUCTION;
		md_mixfreq = 44100;
		if (MikMod_Init("")) {
			SDL_Log("Could not initialize MikMod, reason: %s\n", MikMod_strerror(MikMod_errno));
		}

		SDL_AudioSpec spec = {};
		spec.freq = md_mixfreq;
		spec.format = AUDIO_S16;
		spec.channels = 2;
		spec.samples = 2048;
		spec.callback = sdl_audiocallback;
		if (SDL_OpenAudio(&spec, NULL) < 0) {
			SDL_Log("Could not initialize SDL audio: %s\n", SDL_GetError());
		}

		SDL_PauseAudio(0);

		// load samples
		const char *sample_files[SND_COUNT] = {
			"car", "train", "duck", "pickup1", "tribble", "hurry", "day", "crying",
			"die2", "spit", "splat", "blow", "twinkle", "finlev1", "pstar", "xylo",
			"card", "bowling", "candle", "marble", "tap", "oil", "spiningtop",
			"wings", "moon", "mask", "redstar", "turbo", "chicken", "feather",
			"wpotion", "cookie"
		};

		for (int i = 0; i < SND_COUNT; i++) {
			std::string filename = resource_dir + "sounds/" + sample_files[i] + ".wav";
			m_Soundfile[i] = Sample_Load(filename.c_str());
			if(!m_Soundfile[i]) {
				SDL_Log("Couldn't load %s: %s\n", filename.c_str(), MikMod_strerror(MikMod_errno));
			}
		}

		// load music
		const char *music_files[MODULE_COUNT-1] = {
			"tune1", "tune2", "boss", "complete", "title"
		};

		for (int i = 0; i < MODULE_COUNT-1; i++) {
			std::string filename = resource_dir + "music/" + music_files[i] + ".mod";
			m_Musicfile[i] = Player_Load(filename.c_str(), 64, 0);
			if(!m_Musicfile[i]) {
				SDL_Log("Couldn't load %s: %s\n", filename.c_str(), MikMod_strerror(MikMod_errno));
			}
			m_Musicfile[i]->wrap = true;
		}

		MikMod_SetNumVoices(-1, 16);
	}
}

void CGameTarget::FreeSoundDriver()
{
	if (GLOBAL_SoundEnable)	{
		SDL_PauseAudio(1);

		for (int i = 0; i < SND_COUNT; i++)
			Sample_Free(m_Soundfile[i]);
		for (int i = 0; i < MODULE_COUNT-1; i++)
			Player_Free(m_Musicfile[i]);

		SDL_CloseAudio();
		MikMod_Exit();
	}
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

	// no need to play empty music
	if (id == SMOD_EMPTY) {
		StopModule();
		return;
	}

	// determine music
	MODULE *old = Player_GetModule();
	MODULE *music = m_Musicfile[id - MODULE_START_NUMBER];
	if (!music) {
		SDL_Log("PlayModule: Unknown module.");
		return;
	}

	// keep playing, if next song is the same
	if(old == music) return;

	Player_SetPosition(0);
	Player_Stop();
	Player_Start(music);
}

//------------------------------------------------------------------------------
//! \brief Stop the module (called from the game)
//------------------------------------------------------------------------------
void CGameTarget::StopModule()
{
	if (!GLOBAL_SoundEnable)
		return;

	Player_Stop();
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

	// determine sample chunk
	SAMPLE *sample = m_Soundfile[id - SND_START_NUMBER];
	if (!sample) {
		SDL_Log("PlaySample: Unknown sample.");
		return;
	}

	// Assume centre position is critical (to fix fixed later)
	int flags;
	if ((pos >= 120) && (pos <= 130)) {
		flags = 0;
	} else {
		flags = SFX_CRITICAL;
	}

	int channel = Sample_Play(sample, 0, flags);
	Voice_SetFrequency(channel, rate);
	Voice_SetPanning(channel, pos);

	// The volume wants to be equal while panning left to right
	int volume = 256-8;
	pos = pos - 128; // Centre position
	if (pos < 0) pos = -pos; // Check sign
	volume = volume - pos;
	Voice_SetVolume(channel, volume);
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
		SDL_Log("No idea how to draw white...\n");
	}

	if (SDL_BlitSurface(m_Spritesheet[spritesheet_number], &srcrect, m_Canvas, &dstrect) != 0) {
		SDL_Log("Couldn't blit: %s\n", SDL_GetError());
	}
}
