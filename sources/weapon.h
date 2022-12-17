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
// Methane Brothers Weapon Object (Header File)
//------------------------------------------------------------------------------

#ifndef _WeaponObj_h
#define _WeaponObj_h

#include "objlist.h"
class CMBugObj;
class CMBugWeapon : public CLinkObject
{
public:
	CMBugWeapon();
	virtual ~CMBugWeapon();
	virtual void Do();
	virtual void Draw();
	void Reset();
	void ControlAnim();
public:
	CMBugObj *m_pBaddie;
	int m_Exploding;
};

class CClownObj;
class CClownWeapon : public CLinkObject
{
public:
	CClownWeapon();
	virtual ~CClownWeapon();
	virtual void Do();
	virtual void Draw();
	void Reset();
	void MatchSizeToFrame();
	void MoveWeapon();
public:
	CClownObj *m_pBaddie;
};

#define DWEAPON_STOP	0
#define DWEAPON_GO	1

class CDwarfObj;
class CDwarfWeapon : public CLinkObject
{
public:
	CDwarfWeapon();
	virtual ~CDwarfWeapon();
	virtual void Do();
	virtual void Draw();
	void Reset();
	void MatchSizeToFrame();
	void MoveWeapon();
	void InitThrow( int xpos, int ypos, int dir );
	void DropWeapon();
public:
	CDwarfObj *m_pBaddie;
	int m_Command;
	int m_Ticks;
	int m_OutFlag;
};


class CGeneratorObj : public CLinkObject
{
public:
	CGeneratorObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void Setup();
	int CheckHitByBaddie();
	void CheckReleaseBaddie();
	void CreateBaddie();
	void FindTheBaddieID();
	void ExplodeToys();
	void ControlTimer();
public:
	int m_WhiteFlashCnt;
	int m_HitCnt;
	int m_ReleaseDelay;
	int m_EjectDir;
	int m_EjectXOff;
	int m_EjectYOff;
	int m_EjectCnt;
	int m_EjectFrame;
	int m_BaddieID;	 // GBADID_xxx type
	int m_CheckFlag;
	int m_NumFrame;
	int m_NumXPos;
	int m_NumYPos;
	int m_NumXOff;
	int m_NumYOff;
};

class CBombObj : public CLinkObject
{
public:
	CBombObj();
	virtual void Do();
public:
};


#endif // _WeaponObj_h

