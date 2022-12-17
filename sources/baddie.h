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
// Methane Brothers Bug Object (Header File)
//------------------------------------------------------------------------------

#ifndef _Baddie_h
#define _Baddie_h

#include "objlist.h"
#include "suck.h"

class CPlayerObj;

extern SUCKANIM suck_bug_left[NUMSUCKANIM];
extern SUCKANIM suck_bug_right[NUMSUCKANIM];

class CBugObj : public CSuckable
{
public:
	CBugObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void Reset();
};


class CWhirlyObj : public CSuckable
{
public:
	CWhirlyObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void Reset();
public:
};


class CDoofusObj : public CSuckable
{
public:
	CDoofusObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void Reset();
public:
};


class CJumpObj : public CSuckable
{
public:
	CJumpObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void Reset();
	void DoJump( int frmoffset );
public:
	int m_FixFlag;
	int m_JumpSize;
	int m_ImmumeCnt;
};

extern OBJSIZE jump_size ;
extern OBJMOVE jump_move ;
extern OBJSIZE jump_smallsize ;

extern SUCKFRAMES suck_jump_frames;
extern SUCKFRAMES suck_jump_smallframes;


class CMBugWeapon;

class CMBugObj : public CSuckable
{
public:
	CMBugObj();
	virtual ~CMBugObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void Reset();
	void CheckWeapon();
public:
	CMBugWeapon *m_pWeapon;
};

class CClownWeapon;
class CClownObj : public CSuckable
{
public:
	CClownObj();
	virtual ~CClownObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void Reset();
	void WalkClown();
	int CheckToShoot( CPlayerObj *play );
public:
	CClownWeapon *m_pWeapon;
	int m_ClownCmd;
};

class CDwarfWeapon;
class CDwarfObj : public CSuckable
{
public:
	CDwarfObj();
	virtual ~CDwarfObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void Reset();
	void WalkDwarf();
	int CheckToShoot( CPlayerObj *play );
	virtual void SetupCaught();
public:
	CDwarfWeapon *m_pWeapon;
};

extern SUCKANIM suck_zoom_right[NUMSUCKANIM];
extern SUCKANIM suck_zoom_left[NUMSUCKANIM];
class CZoomObj : public CSuckable
{
public:
	CZoomObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void Reset();
};


class CSpikeObj : public CSuckable
{
public:
	CSpikeObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void Reset();
	void SpikeInAir();
	void SpikeOnGround();
	void Setup( int dir, int xpos, int ypos );
public:
	int m_Length;
};

extern SUCKANIM suck_spike_right[NUMSUCKANIM];
extern SUCKANIM suck_spike_left[NUMSUCKANIM];
extern SUCKANIM suck_sucker_left[NUMSUCKANIM];
extern SUCKANIM suck_sucker_right[NUMSUCKANIM];

class CSuckerObj : public CSuckable
{
public:
	CSuckerObj();
	virtual void Do();
	virtual void Draw();
	void LoadGfx();
	void Reset();
	void MoveSucker();
	void SuckMoveRight();
	void SuckMoveLeft();
	void SuckMoveDown();
	void SuckMoveUp();
public:
	int m_Direction;
};


#endif // _Baddie_h

