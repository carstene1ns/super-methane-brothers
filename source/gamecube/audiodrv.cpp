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
#include "sound.h"

#include "audiodrv.h"

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

    // INIT DRIVER

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
#if 0
	if (!m_AudioValidFlag) return;
	Player_Stop();
#endif
}

//------------------------------------------------------------------------------
//! \brief Play a module (called from the game)
//!
//! 	\param id = SMOD_xxx id
//------------------------------------------------------------------------------
void CAudioDrv::PlayModule(int id)
{
#if 0
	MODULE *mod_ptr;

	if (!m_AudioValidFlag) return;

	StopModule();

	if (m_DisableMusicFlag)
	{
		id = SMOD_EMPTY;
	}

	id = id - MODULE_START_NUMBER;
	if ((id<0) || (id >= MODULE_COUNT)) return;

	mod_ptr = (MODULE *) MethaneModuleData[id].handle;
	if (mod_ptr)
	{
		Player_Start(mod_ptr);
	}
#endif
}

//------------------------------------------------------------------------------
//! \brief Remove Modules
//------------------------------------------------------------------------------
void CAudioDrv::RemoveModules(void)
{
#if 0
	MODULE_RESOURCE_DATA *mptr;
	int cnt;

	mptr = MethaneModuleData;
	for (cnt=0; cnt<MODULE_COUNT; cnt++, mptr++)
	{
		if (mptr->handle)
		{
			Player_Free( (MODULE *) mptr->handle );
			mptr->handle = 0;
		}
	}
#endif
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
#if 0
	MODULE_RESOURCE_DATA *mptr;
	MODULE *module;
	int cnt;

	mptr = MethaneModuleData;

	for (cnt=0; cnt<MODULE_COUNT; cnt++, mptr++)
	{
		MR_Length = mptr->length;
		MR_Data = mptr->ptr;
		MR_Offset = 0;
		module = Player_LoadGeneric(&MethaneReader, 8, 0);
		if (module)
		{
			mptr->handle = module;
		}else	mptr->handle = 0;
	}
#endif
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
#if 0
	if (!m_AudioValidFlag) return;
#endif
}

//------------------------------------------------------------------------------
//! \brief Update the current module (ie restart the module if it has stopped) (called from the game)
//!
//! 	\param id = SMOD_xxx id (The module that should be playing)
//------------------------------------------------------------------------------
void CAudioDrv::UpdateModule(int id)
{
#if 0
	if (!Player_Active())
	{
		Player_SetPosition(0);
		PlayModule(id);
	}
#endif
}
