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
// Sound Driver wrapper (Source File)
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <xmp.h>

#include <3ds.h>

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
#include "blow_wav.h"
#include "bowling_wav.h"
#include "candle_wav.h"
#include "car_wav.h"
#include "card_wav.h"
#include "chicken_wav.h"
#include "cookie_wav.h"
#include "crying_wav.h"
#include "day_wav.h"
#include "die2_wav.h"
#include "duck_wav.h"
#include "feather_wav.h"
#include "finlev1_wav.h"
#include "hurry_wav.h"
#include "marble_wav.h"
#include "mask_wav.h"
#include "moon_wav.h"
#include "oil_wav.h"
#include "pickup1_wav.h"
#include "pstar_wav.h"
#include "redstar_wav.h"
#include "spiningtop_wav.h"
#include "spit_wav.h"
#include "splat_wav.h"
#include "tap_wav.h"
#include "train_wav.h"
#include "tribble_wav.h"
#include "turbo_wav.h"
#include "twinkle_wav.h"
#include "wings_wav.h"
#include "wpotion_wav.h"
#include "xylo_wav.h"

#define SAMPLERATE 22050
#define SAMPLESPERBUF (SAMPLERATE / 30)
#define BYTESPERSAMPLE 4

ndspWaveBuf waveBuf[2];
u32 *audioBuffer;
bool firstblock = true;

// The mod player
static xmp_context methane_music;

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

	audioBuffer = (u32*) linearAlloc(SAMPLESPERBUF * BYTESPERSAMPLE * 2);

	// Initialise the audio subsystem
	ndspInit();
	ndspSetOutputMode(NDSP_OUTPUT_STEREO);
	ndspChnSetInterp(0, NDSP_INTERP_LINEAR);
	ndspChnSetRate(0, SAMPLERATE);
	ndspChnSetFormat(0, NDSP_FORMAT_STEREO_PCM16);

	float mix[12];
	memset(mix, 0, sizeof(mix));
	mix[0] = 1.0;
	mix[1] = 1.0;
	ndspChnSetMix(0, mix);

	memset(waveBuf, 0, sizeof(waveBuf));
	waveBuf[0].data_vaddr = &audioBuffer[0];
	waveBuf[0].nsamples = SAMPLESPERBUF;
	waveBuf[1].data_vaddr = &audioBuffer[SAMPLESPERBUF];
	waveBuf[1].nsamples = SAMPLESPERBUF;

	// Initialise Player
	methane_music = xmp_create_context();

	m_AudioValidFlag = 1;
}

//------------------------------------------------------------------------------
//! \brief Remove the sound driver
//------------------------------------------------------------------------------
void CAudioDrv::RemoveDriver(void)
{
	if (!m_AudioValidFlag) return;

	StopModule();
	xmp_end_player(methane_music);
	xmp_free_context(methane_music);

	ndspExit();
	
	linearFree(audioBuffer);

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
		rate = SAMPLERATE;

#if 0

	switch(id) {
		case SND_CAR:
			sound_ptr = &car_wav;
			sound_length = car_wav_size;
			break;
		case SND_TRAIN:
			sound_ptr = &train_wav;
			sound_length = train_wav_size;
			break;
		case SND_DUCK:
			sound_ptr = &duck_wav;
			sound_length = duck_wav_size;
			break;
		case SND_PICKUP1:
			sound_ptr = &pickup1_wav;
			sound_length = pickup1_wav_size;
			break;
		case SND_TRIBBLE:
			sound_ptr = &tribble_wav;
			sound_length = tribble_wav_size;
			break;
		case SND_HURRY:
			sound_ptr = &hurry_wav;
			sound_length = hurry_wav_size;
			break;
		case SND_DAY:
			sound_ptr = &day_wav;
			sound_length = day_wav_size;
			break;
		case SND_CRYING:
			sound_ptr = &crying_wav;
			sound_length = crying_wav_size;
			break;
		case SND_DIE2:
			sound_ptr = &die2_wav;
			sound_length = die2_wav_size;
			break;
		case SND_SPIT:
			sound_ptr = &spit_wav;
			sound_length = spit_wav_size;
			break;
		case SND_SPLAT:
			sound_ptr = &splat_wav;
			sound_length = splat_wav_size;
			break;
		case SND_BLOW:
			sound_ptr = &blow_wav;
			sound_length = blow_wav_size;
			break;
		case SND_TWINKLE:
			sound_ptr = &twinkle_wav;
			sound_length = twinkle_wav_size;
			break;
		case SND_FINLEV1:
			sound_ptr = &finlev1_wav;
			sound_length = finlev1_wav_size;
			break;
		case SND_PSTAR:
			sound_ptr = &pstar_wav;
			sound_length = pstar_wav_size;
			break;
		case SND_XYLO:
			sound_ptr = &xylo_wav;
			sound_length = xylo_wav_size;
			break;
		case SND_CARDSOUND:
			sound_ptr = &card_wav;
			sound_length = card_wav_size;
			break;
		case SND_BOWLINGSOUND:
			sound_ptr = &bowling_wav;
			sound_length = bowling_wav_size;
			break;
		case SND_CANDLESOUND:
			sound_ptr = &candle_wav;
			sound_length = candle_wav_size;
			break;
		case SND_MARBLESOUND:
			sound_ptr = &marble_wav;
			sound_length = marble_wav_size;
			break;
		case SND_TAPSOUND:
			sound_ptr = &tap_wav;
			sound_length = tap_wav_size;
			break;
		case SND_OILSOUND:
			sound_ptr = &oil_wav;
			sound_length = oil_wav_size;
			break;
		case SND_SPININGTOPSOUND:
			sound_ptr = &spiningtop_wav;
			sound_length = spiningtop_wav_size;
			break;
		case SND_WINGSSOUND:
			sound_ptr = &wings_wav;
			sound_length = wings_wav_size;
			break;
		case SND_MOONSOUND:
			sound_ptr = &moon_wav;
			sound_length = moon_wav_size;
			break;
		case SND_MASKSOUND:
			sound_ptr = &mask_wav;
			sound_length = mask_wav_size;
			break;
		case SND_REDSTARSOUND:
			sound_ptr = &redstar_wav;
			sound_length = redstar_wav_size;
			break;
		case SND_TURBOSOUND:
			sound_ptr = &turbo_wav;
			sound_length = turbo_wav_size;
			break;
		case SND_CHICKENSOUND:
			sound_ptr = &chicken_wav;
			sound_length = chicken_wav_size;
			break;
		case SND_FEATHERSOUND:
			sound_ptr = &feather_wav;
			sound_length = feather_wav_size;
			break;
		case SND_WPOTIONSOUND:
			sound_ptr = &wpotion_wav;
			sound_length = wpotion_wav_size;
			break;
		case SND_COOKIESOUND:
			sound_ptr = &cookie_wav;
			sound_length = cookie_wav_size;
			break;
		default:
			// Error: unknown sample
			return;
	}
#endif

	// Add a voice
	bool this_voice = false;
	
	// Check if voice was available
	if (this_voice) {
		//AESND_PlayVoice(this_voice, VOICE_STEREO16, sound_ptr, sound_length, rate, 0, 0);

		// Do volume math (with fake panning)
		int vol_l = ((255 - pos) > 0) ? (255 - pos) : 0;
		vol_l = (vol_l > 255) ? 255 : vol_l;
		vol_l = (float) sound_volume * (float) vol_l / 10.0;
		
		int vol_r = (pos > 0) ? pos : 0;
		vol_r = (vol_r > 255) ? 255 : vol_r;
		vol_r = (float) sound_volume * (float) pos / 10.0;
		
		//AESND_SetVoiceVolume(this_voice, vol_l, vol_r);
	}
}

//------------------------------------------------------------------------------
//! \brief Stop the module (called from the game)
//------------------------------------------------------------------------------
void CAudioDrv::StopModule(void)
{
	if (!m_AudioValidFlag) return;
	if (m_DisableMusicFlag) return;

	xmp_stop_module(methane_music);
	xmp_release_module(methane_music);
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

	StopModule();

	if (m_DisableMusicFlag) id = SMOD_EMPTY;

	switch(id) {
		case SMOD_TUNE1:
			xmp_load_module_from_memory(methane_music, const_cast<u8*>(tune1_mod), tune1_mod_size);
			break;
		case SMOD_TUNE2:
			xmp_load_module_from_memory(methane_music, const_cast<u8*>(tune2_mod), tune2_mod_size);
			break;
		case SMOD_BOSS:
			xmp_load_module_from_memory(methane_music, const_cast<u8*>(boss_mod), boss_mod_size);
			break;
		case SMOD_COMPLETE:
			xmp_load_module_from_memory(methane_music, const_cast<u8*>(complete_mod), complete_mod_size);
			break;
		case SMOD_TITLE:
			xmp_load_module_from_memory(methane_music, const_cast<u8*>(title_mod), title_mod_size);
			break;
		default:
			xmp_load_module_from_memory(methane_music, const_cast<u8*>(empty_mod), empty_mod_size);
			break;
	}

	// Set the volume, cause it resets on every new mod
	//MODPlay_SetVolume(&methane_music, music_volume, music_volume);
	xmp_start_player(methane_music, SAMPLERATE, 0);
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
	//MODPlay_SetVolume(&methane_music, music_volume, music_volume);
}

void CAudioDrv::Update(void)
{
	int ret = xmp_play_buffer(methane_music, waveBuf[firstblock?0:1].data_pcm16, SAMPLESPERBUF * BYTESPERSAMPLE, 0);
	if (ret == 0) {
		DSP_FlushDataCache(waveBuf[firstblock?0:1].data_pcm16, SAMPLESPERBUF * BYTESPERSAMPLE);
		ndspChnWaveBufAdd(0, &waveBuf[firstblock?0:1]);
		firstblock = !firstblock;
	}
}
