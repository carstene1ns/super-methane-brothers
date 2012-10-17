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

// the mod player
static MODPlay methane_music;

//------------------------------------------------------------------------------
//! \brief Sound driver constructor
//------------------------------------------------------------------------------
CAudioDrv::CAudioDrv()
{
	m_AudioValidFlag = 0;
	m_DisableMusicFlag = 0;
	m_DisableSamplesFlag = 0;
	m_LeftSampleCnt = 0;
	m_RightSampleCnt = 0;
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

    InitModules();
    InitSamples();

    // get ready to play
    //EnableOutput();

    m_AudioValidFlag = 1;
}

//------------------------------------------------------------------------------
//! \brief Remove the sound driver
//------------------------------------------------------------------------------
void CAudioDrv::RemoveDriver(void)
{
	if (!m_AudioValidFlag) return;

	//DisableOutput();
	StopModule();

	RemoveModules();
	RemoveSamples();

	m_AudioValidFlag = 0;
}

//------------------------------------------------------------------------------
//! \brief Play a sample (called from the game)
//!
//! 	\param id = SND_xxx id
//!	\param pos = Sample Position to use 0 to 255
//!	\param rate = The rate
//------------------------------------------------------------------------------
void CAudioDrv::PlaySample(int id, int pos, int rate)
{
#if 0
	SAMPLE *snd_ptr;
	int cid;
	int flags;
	int volume;

	if (!m_AudioValidFlag) return;

	if (m_DisableSamplesFlag) return;

	id = id - SND_START_NUMBER;
	if ((id<0) || (id >= SND_COUNT)) return;
	snd_ptr = (SAMPLE *) MethaneSampleData[id].handle;
	if (snd_ptr)
	{
		// Assume centre position is critical (to fix fixed later)
		if ((pos>=120) && (pos <=130))
		{
			flags = 0;
		}else
		{
			flags = SFX_CRITICAL;
		}

		cid = Sample_Play(snd_ptr, 0, flags);
		//Voice_SetFrequency(cid, rate);
		//Voice_SetPanning(cid, pos);

		// The volume wants to be equal while panning left to right
		volume = 256-8;
		pos = pos - 128;	// Centre position
		if (pos < 0) pos = -pos;	// Check sign
		volume = volume - pos;

		//Voice_SetVolume(cid, volume);
	}
#endif
}

//------------------------------------------------------------------------------
//! \brief Stop the module (called from the game)
//------------------------------------------------------------------------------
void CAudioDrv::StopModule(void)
{
    if (!m_AudioValidFlag) return;

    MODPlay_Stop(&methane_music);
}

//------------------------------------------------------------------------------
//! \brief Play a module (called from the game)
//!
//! 	\param id = SMOD_xxx id
//------------------------------------------------------------------------------
void CAudioDrv::PlayModule(int id)
{
    if (!m_AudioValidFlag) return;

    MODPlay_Stop(&methane_music);

    if (m_DisableMusicFlag) id = SMOD_EMPTY;

    switch(id)
    {
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

    MODPlay_Start(&methane_music);
}

//------------------------------------------------------------------------------
//! \brief Remove Modules
//------------------------------------------------------------------------------
void CAudioDrv::RemoveModules(void)
{
    // not needed
}

//------------------------------------------------------------------------------
//! \brief Remove Samples
//------------------------------------------------------------------------------
void CAudioDrv::RemoveSamples(void)
{
#if 0
	SAMPLE_RESOURCE_DATA *sptr;
	int cnt;

	sptr = MethaneSampleData;
	for (cnt=0; cnt<SND_COUNT; cnt++, sptr++)
	{
		if (sptr->handle)
		{
			Sample_Free( (SAMPLE *) sptr->handle );
			sptr->handle = 0;
		}
	}
#endif
}


//------------------------------------------------------------------------------
//! \brief Initialise the modules
//------------------------------------------------------------------------------
void CAudioDrv::InitModules(void)
{
    // not needed (modules are compiled in)
}

//------------------------------------------------------------------------------
//! \brief Initialise the samples
//------------------------------------------------------------------------------
void CAudioDrv::InitSamples(void)
{
#if 0
	SAMPLE_RESOURCE_DATA *sptr;
	SAMPLE *sample;
	int cnt;

	SignAllSamples();

	sptr = MethaneSampleData;

	for (cnt=0; cnt<SND_COUNT; cnt++, sptr++)
	{
		MR_Length = sptr->length;
		MR_Data = sptr->ptr;
		MR_Offset = 0;

		sample = Sample_LoadGeneric(&MethaneReader);
		if (sample)
		{
			sptr->handle = sample;
		}else	sptr->handle = 0;
	}
	// reserve voices for sound effects
	//SetNumVoices(-1, NUM_SAMPLE_VOICES);
#endif
}

//------------------------------------------------------------------------------
//! \brief Change the sign of all the samples
//------------------------------------------------------------------------------
static int SamplesSignedFlag = 0;
void CAudioDrv::SignAllSamples(void)
{
#if 0
	unsigned char *ptr;
	int len;
	int cnt;

	// Only sign the samples once!
	if (SamplesSignedFlag) return;
	SamplesSignedFlag = 1;

	for (cnt=0; cnt<SND_COUNT; cnt++)	// for all the samples
	{
		ptr = MethaneSampleData[cnt].ptr + WAVE_HEADER_SIZE;
		len = MethaneSampleData[cnt].length - (WAVE_HEADER_SIZE + WAVE_TAIL_SIZE);
		while(len>0)
		{
			*(ptr) = 0x80 - (*ptr);	// Change the sign
			ptr++;
			len--;
		}
	}
#endif
}

//------------------------------------------------------------------------------
//! \brief Call mikmod_update. Call this every cycle
//------------------------------------------------------------------------------
void CAudioDrv::Update(void)
{
    if (!m_AudioValidFlag) return;

    // not needed, MODPlay plays by itself
}

//------------------------------------------------------------------------------
//! \brief Update the current module (ie restart the module if it has stopped) (called from the game)
//!
//! 	\param id = SMOD_xxx id (The module that should be playing)
//------------------------------------------------------------------------------
void CAudioDrv::UpdateModule(int id)
{
    // not really needed, MODPlay loops by itself
    //if (!methane_music.playing) PlayModule(id);
}
