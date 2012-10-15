/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//------------------------------------------------------------------------------
// The MikMod Sound Driver wrapper (Source File)
//------------------------------------------------------------------------------

#ifdef METHANE_MIKMOD

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sound.h"

#include "mikmod.h"
#include "audiodrv.h"

//------------------------------------------------------------------------------
// The Module reader
//------------------------------------------------------------------------------

static unsigned char *MR_Data = 0;	// The sample or module data
static int MR_Length = 0;		// Length of the data
static int MR_Offset = 0;		// Current offset within the data

//------------------------------------------------------------------------------
// Module Reader: fseek()
//------------------------------------------------------------------------------
static BOOL MR_Seek(struct MREADER *rptr, long offset, int code)
{
	if (code==SEEK_SET)
	{
		MR_Offset = offset;
	}
	else if (code==SEEK_CUR)
	{
		MR_Offset += offset;
	}
	else if (code==SEEK_END)
	{
		MR_Offset = MR_Length - offset;
	}

	if (MR_Offset<0) MR_Offset = 0;

	return true;
}
//------------------------------------------------------------------------------
// Module Reader: ftell()
//------------------------------------------------------------------------------
static long MR_Tell(struct MREADER *rptr)
{
	return MR_Offset;
}
//------------------------------------------------------------------------------
// Module Reader: fread()
//------------------------------------------------------------------------------
static BOOL MR_Read(struct MREADER *rptr,void *dest, size_t size)
{
	if (MR_Offset >= (MR_Length- ( (int) size))) return false;
	memcpy(dest, MR_Data + MR_Offset, size);
	MR_Offset+=size;
	return true;
}
//------------------------------------------------------------------------------
// Module Reader: fget()
//------------------------------------------------------------------------------
static int MR_Get(struct MREADER *rptr)
{
	unsigned char *letptr;
	if (MR_Offset >= (MR_Length-1)) return EOF;
	letptr = MR_Data + MR_Offset;
	MR_Offset++;
	return *letptr;
}
//------------------------------------------------------------------------------
// Module Reader: feof()
//------------------------------------------------------------------------------
static BOOL MR_Eof(struct MREADER *rptr)
{
	if (MR_Offset >= MR_Length) return true;
	return false;
}

//------------------------------------------------------------------------------
// Module Reader: Structure
//------------------------------------------------------------------------------
static MREADER MethaneReader = {
	MR_Seek,
	MR_Tell,
	MR_Read,
	MR_Get,
	MR_Eof};

//------------------------------------------------------------------------------
//! \brief Sound driver constructor
//------------------------------------------------------------------------------
CMikModDrv::CMikModDrv()
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
CMikModDrv::~CMikModDrv()
{
	RemoveDriver();
}

//------------------------------------------------------------------------------
//! \brief Initialise the sound driver
//------------------------------------------------------------------------------
void CMikModDrv::InitDriver(void)
{
	RemoveDriver();

#ifdef WIN32	// Should be Windows
	if (drv_ds.Name)	// Valid DS driver
	{
		if (drv_ds.CommandLine)	// Valid Commandline
		{
			drv_ds.CommandLine("buffer=14");
		}
		MikMod_RegisterDriver(&drv_ds);
	}
	MikMod_RegisterDriver(&drv_win);
	MikMod_RegisterDriver(&drv_nos);
#else		// Must be Linux
	if (drv_oss.Name)	// Valid OSS driver
	{
		if (drv_oss.CommandLine)	// Valid Commandline
		{
			drv_oss.CommandLine("buffer=14,count=2");
		}
		MikMod_RegisterDriver(&drv_oss);
	}
	if (drv_alsa.Name)	// Valid ALSA driver
	{
		if (drv_alsa.CommandLine)	// Valid Commandline
		{
			drv_alsa.CommandLine("buffer=14");
		}
		MikMod_RegisterDriver(&drv_alsa);
	}
	MikMod_RegisterDriver(&drv_nos);
#endif

	// register standard tracker
	MikMod_RegisterAllLoaders();

	// Note, the md_mode flags are already set by default
	md_mode |= DMODE_SOFT_SNDFX | DMODE_SOFT_MUSIC;

 	if (MikMod_Init(""))	// Command paramenters are ignored as all drivers are registered
	{
		return;
	}

	InitModules();
	InitSamples();

	// get ready to play
	MikMod_EnableOutput();

	m_AudioValidFlag = 1;
}

//------------------------------------------------------------------------------
//! \brief Remove the sound driver
//------------------------------------------------------------------------------
void CMikModDrv::RemoveDriver(void)
{
	if (!m_AudioValidFlag) return;

	MikMod_DisableOutput();
	StopModule();
	MikMod_Update();

	RemoveModules();
	RemoveSamples();

	MikMod_Update();

	MikMod_Exit();
	m_AudioValidFlag = 0;
}

//------------------------------------------------------------------------------
//! \brief Play a sample (called from the game)
//!
//! 	\param id = SND_xxx id
//!	\param pos = Sample Position to use 0 to 255
//!	\param rate = The rate
//------------------------------------------------------------------------------
void CMikModDrv::PlaySample(int id, int pos, int rate)
{
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
		Voice_SetFrequency(cid, rate);
		Voice_SetPanning(cid, pos);

		// The volume wants to be equal while panning left to right
		volume = 256-8;
		pos = pos - 128;	// Centre position
		if (pos < 0) pos = -pos;	// Check sign
		volume = volume - pos;

		Voice_SetVolume(cid, volume);
	}
}

//------------------------------------------------------------------------------
//! \brief Stop the module (called from the game)
//------------------------------------------------------------------------------
void CMikModDrv::StopModule(void)
{
	if (!m_AudioValidFlag) return;
	Player_Stop();

}

//------------------------------------------------------------------------------
//! \brief Play a module (called from the game)
//!
//! 	\param id = SMOD_xxx id
//------------------------------------------------------------------------------
void CMikModDrv::PlayModule(int id)
{

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

}

//------------------------------------------------------------------------------
//! \brief Remove Modules
//------------------------------------------------------------------------------
void CMikModDrv::RemoveModules(void)
{
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
}

//------------------------------------------------------------------------------
//! \brief Remove Samples
//------------------------------------------------------------------------------
void CMikModDrv::RemoveSamples(void)
{
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
}


//------------------------------------------------------------------------------
//! \brief Initialise the modules
//------------------------------------------------------------------------------
void CMikModDrv::InitModules(void)
{
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
}

//------------------------------------------------------------------------------
//! \brief Initialise the samples
//------------------------------------------------------------------------------
void CMikModDrv::InitSamples(void)
{
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
	MikMod_SetNumVoices(-1, NUM_SAMPLE_VOICES);

}

//------------------------------------------------------------------------------
//! \brief Change the sign of all the samples
//------------------------------------------------------------------------------
static int SamplesSignedFlag = 0;
void CMikModDrv::SignAllSamples(void)
{
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
}

//------------------------------------------------------------------------------
//! \brief Call mikmod_update. Call this every cycle
//------------------------------------------------------------------------------
void CMikModDrv::Update(void)
{
	if (!m_AudioValidFlag) return;
	MikMod_Update();

}

//------------------------------------------------------------------------------
//! \brief Update the current module (ie restart the module if it has stopped) (called from the game)
//!
//! 	\param id = SMOD_xxx id (The module that should be playing)
//------------------------------------------------------------------------------
void CMikModDrv::UpdateModule(int id)
{
	if (!Player_Active())
	{
		Player_SetPosition(0);
		PlayModule(id);
	}
}


#endif // (METHANE_MIKMOD)

