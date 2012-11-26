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
// Sound Driver wrapper (Source File)
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gccore.h>
#include <aesndlib.h>
#include <gcmodplay.h>

#include "sound.h"
#include "audiodrv.h"

// generated mod .h files
#include "title_mod.h"
#include "boss_mod.h"
#include "tune1_mod.h"
#include "tune2_mod.h"
#include "complete_mod.h"
#include "empty_mod.h"

// generated pcm .h files
#include "blow_raw.h"
#include "bowling_raw.h"
#include "candle_raw.h"
#include "car_raw.h"
#include "card_raw.h"
#include "chicken_raw.h"
#include "cookie_raw.h"
#include "crying_raw.h"
#include "day_raw.h"
#include "die2_raw.h"
#include "duck_raw.h"
#include "feather_raw.h"
#include "finlev1_raw.h"
#include "hurry_raw.h"
#include "marble_raw.h"
#include "mask_raw.h"
#include "moon_raw.h"
#include "oil_raw.h"
#include "pickup1_raw.h"
#include "pstar_raw.h"
#include "redstar_raw.h"
#include "spiningtop_raw.h"
#include "spit_raw.h"
#include "splat_raw.h"
#include "tap_raw.h"
#include "train_raw.h"
#include "tribble_raw.h"
#include "turbo_raw.h"
#include "twinkle_raw.h"
#include "wings_raw.h"
#include "wpotion_raw.h"
#include "xylo_raw.h"

// The mod player
static MODPlay methane_music;

// Helper function: Frees the voice if end reached
static void VoiceCallBack(AESNDPB *pb, u32 state)
{
	if (state == VOICE_STATE_STOPPED)
		AESND_FreeVoice(pb);
}

//------------------------------------------------------------------------------
//! \brief Sound driver constructor
//------------------------------------------------------------------------------
CAudioDrv::CAudioDrv()
{
	m_AudioValidFlag = 0;
	sound_volume = 10;
	music_volume = 64;

	// Disables music / sound output
#ifdef NOMUSIC
	m_DisableMusicFlag = 1;
#else
	m_DisableMusicFlag = 0;
#endif
#ifdef NOSOUND
	m_DisableSamplesFlag = 1;
#else
	m_DisableSamplesFlag = 0;
#endif
}

//------------------------------------------------------------------------------
//! \brief Destroy the sound driver
//------------------------------------------------------------------------------
CAudioDrv::~CAudioDrv()
{
	RemoveDriver();
}

//------------------------------------------------------------------------------
//! \brief Initialise the sound driver
//------------------------------------------------------------------------------
void CAudioDrv::InitDriver(void)
{
	RemoveDriver();

	// Initialise the audio subsystem
	AESND_Init();

	// Initialise Player
	MODPlay_Init(&methane_music);

	m_AudioValidFlag = 1;
}

//------------------------------------------------------------------------------
//! \brief Remove the sound driver
//------------------------------------------------------------------------------
void CAudioDrv::RemoveDriver(void)
{
	if (!m_AudioValidFlag) return;

	StopModule();
	AESND_Pause(true);

	m_AudioValidFlag = 0;
}

//------------------------------------------------------------------------------
//! \brief Play a sample (called from the game)
//!
//! \param id = SND_xxx id
//! \param pos = Sample Position to use 0 to 255
//! \param rate = The rate
//------------------------------------------------------------------------------
void CAudioDrv::PlaySample(int id, int pos, int rate)
{
	if (m_DisableSamplesFlag) return;

	const void *sound_ptr = NULL;
	u32 sound_length = 0;

	if (rate == 0)
		rate = VOICE_FREQ32KHZ;

	switch(id) {
		case SND_CAR:
			sound_ptr = &car_raw;
			sound_length = car_raw_size;
			break;
		case SND_TRAIN:
			sound_ptr = &train_raw;
			sound_length = train_raw_size;
			break;
		case SND_DUCK:
			sound_ptr = &duck_raw;
			sound_length = duck_raw_size;
			break;
		case SND_PICKUP1:
			sound_ptr = &pickup1_raw;
			sound_length = pickup1_raw_size;
			break;
		case SND_TRIBBLE:
			sound_ptr = &tribble_raw;
			sound_length = tribble_raw_size;
			break;
		case SND_HURRY:
			sound_ptr = &hurry_raw;
			sound_length = hurry_raw_size;
			break;
		case SND_DAY:
			sound_ptr = &day_raw;
			sound_length = day_raw_size;
			break;
		case SND_CRYING:
			sound_ptr = &crying_raw;
			sound_length = crying_raw_size;
			break;
		case SND_DIE2:
			sound_ptr = &die2_raw;
			sound_length = die2_raw_size;
			break;
		case SND_SPIT:
			sound_ptr = &spit_raw;
			sound_length = spit_raw_size;
			break;
		case SND_SPLAT:
			sound_ptr = &splat_raw;
			sound_length = splat_raw_size;
			break;
		case SND_BLOW:
			sound_ptr = &blow_raw;
			sound_length = blow_raw_size;
			break;
		case SND_TWINKLE:
			sound_ptr = &twinkle_raw;
			sound_length = twinkle_raw_size;
			break;
		case SND_FINLEV1:
			sound_ptr = &finlev1_raw;
			sound_length = finlev1_raw_size;
			break;
		case SND_PSTAR:
			sound_ptr = &pstar_raw;
			sound_length = pstar_raw_size;
			break;
		case SND_XYLO:
			sound_ptr = &xylo_raw;
			sound_length = xylo_raw_size;
			break;
		case SND_CARDSOUND:
			sound_ptr = &card_raw;
			sound_length = card_raw_size;
			break;
		case SND_BOWLINGSOUND:
			sound_ptr = &bowling_raw;
			sound_length = bowling_raw_size;
			break;
		case SND_CANDLESOUND:
			sound_ptr = &candle_raw;
			sound_length = candle_raw_size;
			break;
		case SND_MARBLESOUND:
			sound_ptr = &marble_raw;
			sound_length = marble_raw_size;
			break;
		case SND_TAPSOUND:
			sound_ptr = &tap_raw;
			sound_length = tap_raw_size;
			break;
		case SND_OILSOUND:
			sound_ptr = &oil_raw;
			sound_length = oil_raw_size;
			break;
		case SND_SPININGTOPSOUND:
			sound_ptr = &spiningtop_raw;
			sound_length = spiningtop_raw_size;
			break;
		case SND_WINGSSOUND:
			sound_ptr = &wings_raw;
			sound_length = wings_raw_size;
			break;
		case SND_MOONSOUND:
			sound_ptr = &moon_raw;
			sound_length = moon_raw_size;
			break;
		case SND_MASKSOUND:
			sound_ptr = &mask_raw;
			sound_length = mask_raw_size;
			break;
		case SND_REDSTARSOUND:
			sound_ptr = &redstar_raw;
			sound_length = redstar_raw_size;
			break;
		case SND_TURBOSOUND:
			sound_ptr = &turbo_raw;
			sound_length = turbo_raw_size;
			break;
		case SND_CHICKENSOUND:
			sound_ptr = &chicken_raw;
			sound_length = chicken_raw_size;
			break;
		case SND_FEATHERSOUND:
			sound_ptr = &feather_raw;
			sound_length = feather_raw_size;
			break;
		case SND_WPOTIONSOUND:
			sound_ptr = &wpotion_raw;
			sound_length = wpotion_raw_size;
			break;
		case SND_COOKIESOUND:
			sound_ptr = &cookie_raw;
			sound_length = cookie_raw_size;
			break;

		default:
			// Error: unknown sample
			return;
	}

	// Add a voice
	AESNDPB* this_voice = AESND_AllocateVoice(VoiceCallBack);
	
	// Check if voice was available
	if (this_voice) {
		AESND_PlayVoice(this_voice, VOICE_STEREO16, sound_ptr, sound_length, rate, 0, 0);

		// Do volume math (with fake panning)
		int vol_l = ((255 - pos) > 0) ? (255 - pos) : 0;
		vol_l = (vol_l > 255) ? 255 : vol_l;
		vol_l = (float) sound_volume * (float) vol_l / 10.0;
		
		int vol_r = (pos > 0) ? pos : 0;
		vol_r = (vol_r > 255) ? 255 : vol_r;
		vol_r = (float) sound_volume * (float) pos / 10.0;
		
		AESND_SetVoiceVolume(this_voice, vol_l, vol_r);
	}
}

//------------------------------------------------------------------------------
//! \brief Stop the module (called from the game)
//------------------------------------------------------------------------------
void CAudioDrv::StopModule(void)
{
	if (!m_AudioValidFlag) return;
	if (m_DisableMusicFlag) return;

	MODPlay_Stop(&methane_music);
}

//------------------------------------------------------------------------------
//! \brief Play a module (called from the game)
//!
//! \param id = SMOD_xxx id
//------------------------------------------------------------------------------
void CAudioDrv::PlayModule(int id)
{
	if (!m_AudioValidFlag) return;
	if (m_DisableMusicFlag) return;

	MODPlay_Stop(&methane_music);

	if (m_DisableMusicFlag) id = SMOD_EMPTY;

	switch(id) {
		case SMOD_TUNE1:
			MODPlay_SetMOD(&methane_music, tune1_mod);
			break;
		case SMOD_TUNE2:
			MODPlay_SetMOD(&methane_music, tune2_mod);
			break;
		case SMOD_BOSS:
			MODPlay_SetMOD(&methane_music, boss_mod);
			break;
		case SMOD_COMPLETE:
			MODPlay_SetMOD(&methane_music, complete_mod);
			break;
		case SMOD_TITLE:
			MODPlay_SetMOD(&methane_music, title_mod);
			break;
		default:
			MODPlay_SetMOD(&methane_music, empty_mod);
			break;
	}

	// Set the volume, cause it resets on every new mod
	MODPlay_SetVolume(&methane_music, music_volume, music_volume);
	MODPlay_Start(&methane_music);
}

//------------------------------------------------------------------------------
//! \brief Change the music and sound volume
//! \param s = sound volume (0 to 10)
//! \param m = music volume (0 to 10)
//------------------------------------------------------------------------------
void CAudioDrv::ChangeVolume(int s, int m)
{
	sound_volume = s;
	music_volume = m * 6.4;

	// Set the volume for the current mod
	MODPlay_SetVolume(&methane_music, music_volume, music_volume);
}
