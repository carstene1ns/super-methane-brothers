/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * Program WebSite: http://methane.sourceforge.net/index.html              *
 * Email: rombust@postmaster.co.uk                                         *
 *                                                                         *
 ***************************************************************************/

//------------------------------------------------------------------------------
// Methane Brothers Sound Control (Source File)
//------------------------------------------------------------------------------

#ifdef METHANE_OLD_CPP
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#else
#include <cstdlib>
#include <cstdio>
#include <cstring>
#endif

#include "sound.h"
#include "target.h"

//------------------------------------------------------------------------------
//! \brief Sound driver constructor
//------------------------------------------------------------------------------
CSoundDrv::CSoundDrv()
{
	m_CurrentModule = 0;
}

//------------------------------------------------------------------------------
//! \brief Destroy the sound driver
//------------------------------------------------------------------------------
CSoundDrv::~CSoundDrv()
{
}

//------------------------------------------------------------------------------
//! \brief Prepare all audio to play
//------------------------------------------------------------------------------
void CSoundDrv::PrepareAudio(void)
{
}

//------------------------------------------------------------------------------
//! \brief Stop the module from playing
//------------------------------------------------------------------------------
void CSoundDrv::StopModule(void)
{
	GLOBAL_GameTarget->StopModule();
	m_CurrentModule = 0;
}

//------------------------------------------------------------------------------
//! \brief Play the module, or restart playing the module
//!
//! 	\param id = Module id (SMOD_xxx id)
//------------------------------------------------------------------------------
void CSoundDrv::PlayModule(int id)
{
	m_CurrentModule = id;
	GLOBAL_GameTarget->PlayModule(id);
}

//------------------------------------------------------------------------------
//! \brief Play the sample
//!
//! 	\param id = SND_xxx sound id
//!	\param pos = Sample Position to use 0 to SCR_WIDTH-1. (Default = SCR_WIDTH/2)
//!	\param rate = sound frequency (Default = 0 = Use sample default)
//------------------------------------------------------------------------------
void CSoundDrv::PlaySample(int id, int pos, int rate)
{
	int offset;

	if ( (id<SND_START_NUMBER) || (id>SND_END_NUMBER) ) return;
	offset = id - SND_START_NUMBER;

	if (!rate) rate = MethaneSampleData[offset].rate;

	// Scale the position from 0 to 255
	if (pos<0) pos = 0;
	if (pos>=SCR_WIDTH) pos = SCR_WIDTH-1;

	pos = (pos << 8) / SCR_WIDTH;

	GLOBAL_GameTarget->PlaySample(id, pos, rate);
}

//------------------------------------------------------------------------------
//! \brief Update the current module (ie restart the module if it has stopped)
//------------------------------------------------------------------------------
void CSoundDrv::UpdateModule(void)
{
	if (m_CurrentModule)	// Is something playing
	{
		GLOBAL_GameTarget->UpdateModule(m_CurrentModule);
	}
}


