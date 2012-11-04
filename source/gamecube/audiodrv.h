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
 * GameCube Port by Infact <infact quantentunnel de> 10/2012
 */

//------------------------------------------------------------------------------
// Sound Driver wrapper (Header File)
//------------------------------------------------------------------------------

#ifndef _audiodrv_h
#define _audiodrv_h 1

#include "snddef.h"

class CAudioDrv
{
public:
	CAudioDrv();
	~CAudioDrv();
	void InitDriver(void);
	void RemoveDriver(void);
	void PlaySample(int id, int pos, int rate);
	void StopModule(void);
	void PlayModule(int id);
	int m_AudioValidFlag;
	int m_DisableMusicFlag;
	int m_DisableSamplesFlag;
};

#endif
