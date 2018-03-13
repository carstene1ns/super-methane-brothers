/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * Program WebSite: http://www.methane.fsnet.co.uk/index.html              *
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
// Sound driver constructor
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
CSoundDrv::CSoundDrv()
{
	m_CurrentModule = 0;
}

//------------------------------------------------------------------------------
// Destroy the sound driver
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
CSoundDrv::~CSoundDrv()
{
}

//------------------------------------------------------------------------------
// Prepare all audio to play
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CSoundDrv::PrepareAudio(void)
{
}

//------------------------------------------------------------------------------
// Stop the module from playing
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CSoundDrv::StopModule(void)
{
	GLOBAL_GameTarget->StopModule();
	m_CurrentModule = 0;
}

//------------------------------------------------------------------------------
// Play the module, or restart playing the module
// On Entry:
// 	id = Module id (SMOD_xxx id)
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CSoundDrv::PlayModule(int id)
{
	m_CurrentModule = id;
	GLOBAL_GameTarget->PlayModule(id);
}

//------------------------------------------------------------------------------
// Play the sample
// On Entry:
// 	id = SND_xxx sound id
//	pos = Sample Position to use 0 to SCR_WIDTH-1. (Default = SCR_WIDTH/2)
//	rate = sound frequency (Default = 0 = Use sample default)
// On Exit:
// 	Not Used
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
// Update the current module (ie restart the module if it has stopped)
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CSoundDrv::UpdateModule(void)
{
	if (m_CurrentModule)	// Is something playing
	{
		GLOBAL_GameTarget->UpdateModule(m_CurrentModule);
	}
}


