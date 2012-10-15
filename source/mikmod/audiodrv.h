/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * Program WebSite: http://methane.sourceforge.net/index.html              *
 * Prgram Email: rombust@postmaster.co.uk                                  *
 *                                                                         *
 ***************************************************************************/

//------------------------------------------------------------------------------
// The MikMod Sound Driver wrapper (Header File)
//------------------------------------------------------------------------------

#ifdef METHANE_MIKMOD

#ifndef _audiodrv_h
#define _audiodrv_h 1

#include "snddef.h"

#define NUM_SAMPLE_VOICES	12

class CMikModDrv
{
public:
	CMikModDrv();
	~CMikModDrv();
	void InitDriver(void);
	void RemoveDriver(void);
	void PlaySample(int id, int pos, int rate);
	void StopModule(void);
	void PlayModule(int id);
	void Update(void);
	void UpdateModule(int id);

private:
	void RemoveModules(void);
	void RemoveSamples(void);
	void InitModules(void);
	void InitSamples(void);
	void SignAllSamples(void);

private:
	int m_LeftSampleCnt;
	int m_RightSampleCnt;

public:
	int m_AudioValidFlag;
	int m_DisableMusicFlag;
	int m_DisableSamplesFlag;

};

#endif
#endif // (METHANE_MIKMOD)
