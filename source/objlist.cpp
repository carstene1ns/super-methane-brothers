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
// Methane Brothers Object lists (Source File)
//------------------------------------------------------------------------------

#ifdef METHANE_OLD_CPP
#include <stdlib.h>
#else
#include <cstdlib>
#endif

#include "objlist.h"
#include "global.h"
#include "player.h"
#include "game.h"
#include "bitgroup.h"
#include "objtypes.h"

//------------------------------------------------------------------------------
// Initialise the link object
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
CLinkObject::CLinkObject()
{
	m_pListBase = 0;
	m_pNext = m_pPrev = 0; m_Type = 0;		// Clear members
	Reset();
}

//------------------------------------------------------------------------------
// Destroy the link object (calls unlink)
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
CLinkObject::~CLinkObject()
{
	UnLink();
}

//------------------------------------------------------------------------------
// Reset the link object - by clearing it's data members
// (Object links are not cleared)
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::Reset( void )
{
	m_XPos = m_YPos = m_OldXPos = m_OldYPos = m_Dir = 0;
	m_Frame = m_XInert = m_XPower = m_XMass = m_Max_XSpeed = 0;
	m_YInert = m_YPower = m_YMass = m_Max_YSpeed = m_YCentre = 0;
	m_Y_Flag = -1;
	m_Delay = 0;
	m_pSequence = 0;
	m_Seq_Offset = m_X1 = m_Y1 = 0;
	m_X2 = m_Y2 = m_State = 0;
	m_Flags = 0;
	m_YDir = 0;
}

//------------------------------------------------------------------------------
// Delete this object. (Use this is preference of clearing obj_type)
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::DeleteObject(void)
{
	m_Type = OBJ_DEAD;
// The below line was removed because, in certain cases, the object is
// unlinked and left in memory. Unlink is called in the destructor instead
//	UnLink();

}

//------------------------------------------------------------------------------
// Do an object (objects main loop) (VIRTUAL)
// DOES NOTHING
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::Do(void)
{

}

//------------------------------------------------------------------------------
// Draw an object (VIRTUAL)
// DRAWS m_Frame
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::Draw(void)
{
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );

}

//------------------------------------------------------------------------------
// Unlink the link object
// WARNING: Remember to destroy this object manually if the 'calling function'
//  does not call LinkBefore()/LinkAfter() - Else memory leaks will occur
// Only use this function to destroy an object if the main objectlist
//  is also being cleared.
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::UnLink( void )
{
	if (m_pListBase)
	{
		if (m_pListBase->m_pFirst == this)	// If first link, then set to next
		{
			m_pListBase->m_pFirst = m_pNext;	// Need to keep the address of the first link
		}
	}

	if (m_pNext)				// Is there a next link?
	{
		m_pNext->m_pPrev = m_pPrev;	// Link next object to previous object
	}

	if (m_pPrev)				// Is there a previous link?
	{
		m_pPrev->m_pNext = m_pNext;	// Link previous object to next object
	}
	m_pNext = m_pPrev = 0;

}

//------------------------------------------------------------------------------
// Link this object to the start of the object list
// This function uses m_pListBase to attach to
// (This object MUST already be unlinked)
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::Link( void )
{
	if (m_pListBase)					// Must be valid
	{
		if (m_pListBase->m_pFirst)	// If there already a link there?
		{
			LinkBefore(m_pListBase->m_pFirst);
		}else
		{
			m_pListBase->m_pFirst = this;	// Store this first link address
		}
	}
}


//------------------------------------------------------------------------------
// Link this link object before an object
// (This object must already be unlinked)
// On Entry:
// 	linkto = what to link to (must be a valid object)
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::LinkBefore( CLinkObject *linkto )
{
	m_pNext = linkto;			// Link in object
	m_pPrev = linkto->m_pPrev;	// Copy previous object

	linkto->m_pPrev = (this);	// Set next object - previous
	if (m_pPrev)				// Only if exists
	{
		m_pPrev->m_pNext = (this);	// Set previous next object
	}else						// This is now the first in the list
	{
		if (m_pListBase)
			m_pListBase->m_pFirst = this;	// Store address of this first object
	}
}

//------------------------------------------------------------------------------
// Link this link object after an object
// (This object must already be unlinked)
// On Entry:
// 	linkto = what to link to (must be a valid object)
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::LinkAfter( CLinkObject *linkto )
{
	m_pPrev = linkto;			// Link in object
	m_pNext = linkto->m_pNext;	// Copy next object

	linkto->m_pNext = (this);	// Set previous object - next

	if (m_pNext)				// Only if exists
	{
		m_pNext->m_pPrev = (this);	// Set next previous object

	}
}
//------------------------------------------------------------------------------
// Animate the object using the X inertia as the anim rate
// On Entry:
// 	seq_left = anim facing left
// 	seq_right = anim facing right
//		scale (OPTIONAL, DEFAULT = 2) = Inertia Scale (Bit shift right)
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::AnimateXInert( int *seq_left, int *seq_right, int scale )
{
	int animrate;
	animrate = m_XInert;
	if (animrate<0) animrate = -animrate;
	animrate = animrate >> scale;
	AnimateDir(seq_left, seq_right, animrate);

}

//------------------------------------------------------------------------------
// Animate an object, taking account of the direction
// If the animation frame changes, the m_Seq_Offset is set to zero
// On Entry:
// 	seq_left = anim facing left
// 	seq_right = anim facing right
//		rate = animation rate (256 = full speed). (8 Bit Fraction)
//		flags (optional). ANMFLG_NORESTART = Don't clear offset if anim changed
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::AnimateDir( int *seq_left, int *seq_right, int rate, int flags )
{
	int *newfrm;
	if (m_Dir==DIR_LEFT)
	{
		newfrm = seq_left;
	}else
	{
		newfrm = seq_right;
	}
	if (newfrm!=m_pSequence)		// Anim changed?
	{
		m_pSequence = newfrm;	// Set new frame
		if (!flags)
		{
			m_Seq_Offset = 0;		// Reset offset
			rate = 256;				// Force animation change
		}
	}
	Animate(rate);
}

//------------------------------------------------------------------------------
// Set a new animation to be displayed so that
//  the animation will start straight away at the next animate() call
// On Entry:
// 	seq_ptr = address of the sequence
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::SetAnim(int *seq_ptr)
{
	m_pSequence = seq_ptr;
	m_Seq_Offset = 0;		// Reset offset
	m_Delay = 256-1;		// Force animation change
	m_Frame = *seq_ptr;		// Set first frame
}

//------------------------------------------------------------------------------
// Animate an object
// If sequence = 0, then this function does nothing
// m_State flags used: ANM_RESTART
// On Entry:
// 	rate = anim rate (256 = full speed). (8 Bit Fraction)
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::Animate( int rate )
{
	int animframe;
	int olddelay;
	if (!m_pSequence) return;

	m_State = m_State & (~STATE_ANM_RESTART);
	olddelay = m_Delay >> 8;
	m_Delay+=rate;
	if ((m_Delay>>8) != olddelay)
	{
		while(1)
		{
			animframe  = m_pSequence[m_Seq_Offset++];	// Get anim frame
			switch (animframe)
			{
				case (ANM_RESTART):
				{
					m_Seq_Offset = 0;
					m_State |= STATE_ANM_RESTART;
					break;	// (Get the first frame again)
				}
				case (ANM_PAUSE):
				{
					m_Seq_Offset--;
					return;
				}
				default:
				{
					m_Frame = animframe;				// Copy frame
					return;
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
// Set an object to move left
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::SetMoveLeft( void )
{
	m_XInert = m_XInert - m_XPower;
	if (m_XInert < (-m_Max_XSpeed) )
	{
		m_XInert = -m_Max_XSpeed;
	}
	m_Dir = DIR_LEFT;
}

//------------------------------------------------------------------------------
// Set an object to move right
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::SetMoveRight( void )
{
	m_XInert = m_XInert + m_XPower;
	if (m_XInert > m_Max_XSpeed )
	{
		m_XInert = m_Max_XSpeed;
	}
	m_Dir = DIR_RIGHT;
}


//------------------------------------------------------------------------------
// Set an object to move up (Sets m_Y_Flag)
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::SetMoveUp( void )
{
	m_YInert = m_YInert - m_YPower;
	if (m_YInert < (-m_Max_YSpeed) )
	{
		m_YInert = -m_Max_YSpeed;
	}
	m_Y_Flag = 1;
}

//------------------------------------------------------------------------------
// Set an object to move down
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::SetMoveDown( void )
{
	m_YInert = m_YInert + m_YPower;
	if (m_YInert > m_Max_YSpeed )
	{
		m_YInert = m_Max_YSpeed;
	}
}

//------------------------------------------------------------------------------
// Move the object using the inertias (Y Axis)
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::DoYInertia( void )
{
	int delta;
	if (m_YInert != m_YCentre)			// is not terminal velocity?
	{
		if (m_YInert >= m_YCentre)
		{
			m_YInert-=m_YMass;
			if (m_YInert<m_YCentre) m_YInert = m_YCentre;
		}else
		{
			m_YInert+=m_YMass;
			if (m_YInert>m_YCentre) m_YInert = m_YCentre;
		}
	}

	delta = m_YInert>>8;	// Get inertia value
// - REMOVED FOR COMPATIBILITY
//	if (delta<0) delta++;	// Correct left movement (0 or -1 = stopped)
	m_YPos += delta;

}

//------------------------------------------------------------------------------
// Move the object using the inertias (X Axis)
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::DoXInertia( void )
{
	int delta;
	if (m_XInert)		// Only need to do if the object is moving
	{
		if (m_XInert>0)
		{
			m_XInert-=m_XMass; if (m_XInert<0) m_XInert = 0;
		}else
		{
			m_XInert+=m_XMass; if (m_XInert>0) m_XInert = 0;
		}

		delta = m_XInert>>8;	// Get inertia value

		if (delta<0) delta++;	// Correct left movement (0 or -1 = stopped)

		m_XPos += delta;
	}

}

//------------------------------------------------------------------------------
// Check to see if the object can move in the Y axis, without hitting a wall
// The oldxpos, xpos, oldypos, ypos members are used for the check
// m_State flags used: UP,DOWN
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::DoYCheck( void )
{
	int delta;
	m_State = m_State & (~STATE_VERT);
	if (m_YPos > m_OldYPos)
	{
		delta=CheckDown();
		if (delta!=CBM_NOTINWALL)
		{
			m_YPos = delta;
			m_Y_Flag = 0;
			m_YInert = 0;
			m_State |= STATE_DOWN;
		}
	}else
	if (m_YPos < m_OldYPos)
	{
		delta=CheckUp();
		if (delta!=CBM_NOTINWALL)
		{
			m_YPos = delta;
			m_YInert = 0;
			m_State |= STATE_UP;
		}
	}

	if (m_YInert)				// is not on surface
	{
		m_Y_Flag = 1;			// set jump flag
	}

}

//------------------------------------------------------------------------------
// Check to see if the object can move in the X axis, without hitting a wall
// The oldxpos, xpos, oldypos, ypos members are used for the check
// m_State flags used: LEFT,RIGHT
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::DoXCheck( void )
{
	int delta;
	m_State = m_State & (~STATE_HORIZ);
	if (m_XPos < m_OldXPos)
	{
		delta=CheckLeft();
		if (delta!=CBM_NOTINWALL)
		{
			m_XPos = delta;
			m_XInert = 0;
			m_State |= STATE_LEFT;
		}
	}
	if (m_XPos > m_OldXPos)
	{
		delta=CheckRight();
		if (delta!=CBM_NOTINWALL)
		{
			m_XPos = delta;
			m_XInert = 0;
			m_State |= STATE_RIGHT;
		}
	}
}

//------------------------------------------------------------------------------
// Move the object using it's inertia values
// This function calls ::CheckPos on completion
// m_State flags used: LEFT,RIGHT,UP,DOWN
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::MoveObject( void )
{
	m_OldXPos = m_XPos;
	m_OldYPos = m_YPos;

	if (m_Flags & FLAG_ACCURATE)
	{
		DoXInertia();
		DoYInertia();
	
		// Add accuracy when landing on platforms when falling fast
		int diff_y = m_YPos - m_OldYPos;
		if (diff_y >= 4)	// Falling too fast down
		{
			m_Y1 += 8;
			m_Y2 -= 8;
			DoXCheck();
			m_Y1 -= 8;
			m_Y2 += 8;
			DoYCheck();
		}else
		{
			DoXCheck();
			DoYCheck();
		}
	}else
	{

		DoXInertia();
		DoXCheck();
		DoYInertia();
		DoYCheck();

	}
	::CheckPos(m_XPos, m_YPos);
}

//------------------------------------------------------------------------------
// Move the object using it's Y inertia values
// This function calls ::CheckPos on completion
// m_State flags used: UP,DOWN
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::MoveObject_Y( void )
{
	m_OldYPos = m_YPos;

	DoYInertia();
	DoYCheck();

	::CheckPos(m_XPos, m_YPos);
}


//------------------------------------------------------------------------------
// Check to see if an object will move down into a wall
// On Entry:
// 	Not Used
// On Exit:
//		New m_YPos to place the object
//		CBM_NOTINWALL = Did not hit a wall
//------------------------------------------------------------------------------
int CLinkObject::CheckDown( void )
{
	int yp;

	yp = m_pGame->m_Map.CheckDown(m_XPos + m_X1, m_YPos + m_Y1 + m_Y2,
						m_X2, m_YPos - m_OldYPos);
	if (yp==CBM_NOTINWALL) return yp;

	yp = yp - 1 - m_Y2 - m_Y1;
	return yp;

}

//------------------------------------------------------------------------------
// Check to see if an object will move up into a wall
// On Entry:
// 	Not Used
// On Exit:
//		New m_YPos to place the object
//		CBM_NOTINWALL = Did not hit a wall
//------------------------------------------------------------------------------
int CLinkObject::CheckUp( void )
{
	int yp;

	yp = m_pGame->m_Map.CheckUp(m_XPos + m_X1, m_YPos + m_Y1,
						m_X2, m_YPos - m_OldYPos,m_Flags & FLAG_HARDWALL);
	if (yp==CBM_NOTINWALL) return yp;

	yp = yp - m_Y1 + 1;
	return yp;

}
//------------------------------------------------------------------------------
// Check to see if an object will move left into a wall
// On Entry:
// 	Not Used
// On Exit:
//		New m_XPos to place the object
//		CBM_NOTINWALL = Did not hit a wall
//------------------------------------------------------------------------------
int CLinkObject::CheckLeft( void )
{
	int xp;

	xp = m_pGame->m_Map.CheckLeft(m_XPos + m_X1, m_YPos + m_Y1,
						m_Y2, m_XPos - m_OldXPos,m_Flags & FLAG_HARDWALL);
	if (xp==CBM_NOTINWALL) return xp;

	xp = xp - m_X1 + 1;
	return xp;

}

//------------------------------------------------------------------------------
// Check to see if an object will move right into a wall
// On Entry:
// 	Not Used
// On Exit:
//		New m_XPos to place the object
//		CBM_NOTINWALL = Did not hit a wall
//------------------------------------------------------------------------------
int CLinkObject::CheckRight( void )
{
	int xp;

	xp = m_pGame->m_Map.CheckRight(m_XPos + m_X1 + m_X2, m_YPos + m_Y1,
						m_Y2, m_XPos - m_OldXPos,m_Flags & FLAG_HARDWALL);
	if (xp==CBM_NOTINWALL) return xp;

	xp = xp - 1 - m_X2 - m_X1;
	return xp;

}
//------------------------------------------------------------------------------
// Check to see if an object has hit a wall
// On Entry:
// 	Not Used
// On Exit:
// 	0 = Did not hit a wall
//------------------------------------------------------------------------------
BOXPOS *CLinkObject::CheckHitWall( void )
{
	return m_pGame->m_Map.CheckHitWall(m_XPos+m_X1,
		m_YPos+m_Y1, m_X2, m_Y2, m_Flags & FLAG_HARDWALL);

}

//------------------------------------------------------------------------------
// Check to see if an object has hit this one - Taking account of the
//  screen wrapping.
// On Entry:
//		b_x1,b_y1 = Offset coordinates
//		b_width, b_height = Object width, height
// On Exit:
//		0 = No Collision occured
//------------------------------------------------------------------------------
int CLinkObject::CheckHit(int b_x1,int b_y1, int b_width, int b_height )
{
	int a_x1,a_x2,a_y1,a_y2;
	int b_x2,b_y2;
	int ta_x1,ta_x2;
	int tb_x1,tb_x2;
	int jump_flag;

	a_x1 = m_X1 + m_XPos;	// Extract this position
	a_y1 = m_Y1 + m_YPos;

	::CheckPos(a_x1,a_y1);
	::CheckPos(b_x1,b_y1);

	a_x2 = a_x1 + m_X2;
	a_y2 = a_y1 + m_Y2;

	b_x2 = b_x1 + b_width;
	b_y2 = b_y1 + b_height;

	jump_flag = 1;
	ta_x1 = a_x1;		// Save variables
	ta_x2 = a_x2;
	tb_x1 = b_x1;
	tb_x2 = b_x2;

	goto bsr_ch_loop;	// Test for normal Y
ch_loop:;
	a_x1= ta_x1 ;		// Recover variables
	a_x2= ta_x2 ;
	b_x1= tb_x1 ;
	b_x2= tb_x2 ;

	if (a_y1 < b_y1)		// Check for 'gone off screen' Y
	{
		a_y1 += SCR_HEIGHT;
		a_y2 += SCR_HEIGHT;
	}else
	{
		b_y1 += SCR_HEIGHT;
		b_y2 += SCR_HEIGHT;
	}

bsr_ch_loop:;
	if ((a_x1 < b_x2) && (a_y1 < b_y2) &&
		(a_x2 >= b_x1) && (a_y2 >= b_y1) ) return 1;	// Normal Test

	if (a_x1 < b_x1)		// Check for 'gone off screen' X
	{
		a_x1 += SCR_WIDTH;
		a_x2 += SCR_WIDTH;
	}else
	{
		b_x1 += SCR_WIDTH;
		b_x2 += SCR_WIDTH;
	}

	if ((a_x1 < b_x2) && (a_y1 < b_y2) &&
		(a_x2 >= b_x1) && (a_y2 >= b_y1) ) return 1;	// Secondary Test

	if (jump_flag)
	{
		jump_flag = 0;
		goto ch_loop;
	}

	return 0;


}
//------------------------------------------------------------------------------
// Check to see if an object has hit this one - Taking account of the
//  screen wrapping.
// On Entry:
// 	pobj = object to check with
// On Exit:
//		0 = No Collision occured
//------------------------------------------------------------------------------
int CLinkObject::CheckHit( CLinkObject *pobj )
{
	return (CheckHit(pobj->m_X1+pobj->m_XPos, pobj->m_Y1+pobj->m_YPos,
					pobj->m_X2, pobj->m_Y2));
}


//------------------------------------------------------------------------------
// Set the object size members
// On Entry:
// 	zsize (REFERENCE) = Size Structure
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::InitSize( OBJSIZE &zsize )
{
	m_X1 = zsize.x1;
	m_Y1 = zsize.y1;
	m_X2 = zsize.x2;
	m_Y2 = zsize.y2;
}

//------------------------------------------------------------------------------
// Set the object movement members
// On Entry:
// 	zmove (REFERENCE) = Movement Structure
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::InitMovement( OBJMOVE &zmove )
{
	m_XPower = zmove.xpower;
	m_XMass = zmove.xmass;
	m_Max_XSpeed = zmove.max_xspeed;
	m_YPower = zmove.ypower;
	m_YMass = zmove.ymass;
	m_Max_YSpeed = zmove.max_yspeed;
	m_YCentre = zmove.ycentre;
}

//------------------------------------------------------------------------------
// Move the object left and right, turning at walls
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::DirWalk( void )
{
	if (m_State&STATE_LEFT) m_Dir = DIR_RIGHT;
	if (m_State&STATE_RIGHT) m_Dir = DIR_LEFT;
	if (m_Dir==DIR_LEFT)
	{
		SetMoveLeft();
	}else
	{
		SetMoveRight();
	}

}

//------------------------------------------------------------------------------
// Fly the object left and right, up and down, turning at walls
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::DirFly( void )
{
	if (!m_YDir)	// Going Up?
	{
		SetMoveUp();
	}else
	{
		SetMoveDown();
	}

	if (!m_Dir)	// Going Left?
	{
		SetMoveLeft();
	}else
	{
		SetMoveRight();
	}

	MoveObject();

	if (m_State&STATE_LEFT) m_Dir = DIR_RIGHT;
	if (m_State&STATE_RIGHT) m_Dir = DIR_LEFT;
	if (m_State&STATE_UP) m_YDir = 1;
	if (m_State&STATE_DOWN) m_YDir = 0;

}


//------------------------------------------------------------------------------
// Jump towards the player if the player is above this object
// The jump occurs randomly
// On Entry:
// 	rndmask = bitmask to determine the random interval
//			ie. 31 is '1 in 32 chance'
//		yoff = offset added to the player object
//			which is used to compare the y position (try 8)
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::Jump2Player( int rndmask, int yoff )
{
	int yoffset;
	CLinkObject *pobj;

	if (!(rand()%rndmask))
	{
		pobj = m_pGame->m_PlayerList.m_pFirst;
		while (pobj)
		{
			yoffset = pobj->m_YPos;
			yoffset = (yoffset + yoff) >> 3;

			if ( (m_YPos>>3) >yoffset )	// player above baddie?
			{
				SetMoveUp();
				break;
			}
			pobj = pobj->m_pNext;
		}
	}

}

//------------------------------------------------------------------------------
// Check to see if an object has collided with the player object
// On Entry:
// 	immune_flag: (Default = 0) 1 = Do not collide if the player is immune
// On Exit:
// 	Pointer to the link object (IE the player)
//------------------------------------------------------------------------------
CPlayerObj* CLinkObject::HitPlayer( int immune_flag )
{
	int type;
	int flags;
	CPlayerObj *pobj;
	pobj = (CPlayerObj *) m_pGame->m_PlayerList.m_pFirst;
	for (;pobj;pobj = (CPlayerObj *) pobj->m_pNext)
	{
		type = pobj->m_Type;
		if ( (type==OBJ_PLAYER_ONE) || (type==OBJ_PLAYER_TWO) )
		{
			if (pobj->m_Command==PCMD_RISEUP)	// Nearing completion?
			{
				if (pobj->m_YPos<=-16) continue;	// Moving off top?
			}
			flags = pobj->m_PlayerFlags;
			if (flags&PF_IS_ALIVE)
			{
				if (immune_flag) if (flags&PF_IS_IMMUNE) continue;
				if (CheckHit( pobj ))
				{
					return pobj;
				}
			}
		}
	}
	return 0;
}

//------------------------------------------------------------------------------
// Check to see if an object has collided with the player object
// If a collision has occured - Kill the player
// On Entry:
// 	Not Used
// On Exit:
// 	Pointer to the link object (IE the player)
//------------------------------------------------------------------------------
CPlayerObj* CLinkObject::CheckKillPlayer( void )
{
	CPlayerObj *pobj = HitPlayer(1);
	if (pobj)
	{
		pobj->KillPlayer();
	}
	return pobj;

}

//------------------------------------------------------------------------------
// Fly the object towards the player
// On Entry:
// 	play = Player to fly towards
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CLinkObject::Fly2Player( CPlayerObj *play )
{
	int thisx,thisy;
	int playx,playy;

	if (play)
	{
		thisx = m_XPos;
		thisy = m_YPos;
		playx = play->m_XPos;
		playy = play->m_YPos;

		thisx = thisx & (~1);	// From AMIGA - Do not know why!
		thisy = thisy & (~1);
		playx = playx & (~1);
		playy = playy & (~1);

		thisx = thisx - playx;
		if (thisx<0)				// Negative X
		{
			if (thisx<(-(SCR_WIDTH/2)))	// Too far left?
			{
				thisx+=SCR_WIDTH;		// Make positive
			}
		}else
		{
			if (thisx>((SCR_WIDTH/2)))		// Too far right?
			{
				thisx-=SCR_WIDTH;			// Make negative
			}
		}

		thisy = thisy - playy;
		if (thisy<0)				// Negative X
		{
			if (thisy<(-(SCR_HEIGHT/2)))	// Too far left?
			{
				thisy+=SCR_HEIGHT;		// Make positive
			}
		}else
		{
			if (thisy>((SCR_HEIGHT/2)))		// Too far right?
			{
				thisy-=SCR_HEIGHT;			// Make negative
			}
		}

		if (thisx)		// Move in X Axis?
		{
			if (thisx>0)
			{
				SetMoveLeft();
			}else
			{
				SetMoveRight();
			}
		}

		if (thisy)		// Move in Y Axis?
		{
			if (thisy>0)
			{
				SetMoveUp();
			}else
			{
				SetMoveDown();
			}
		}

	}


}

//------------------------------------------------------------------------------
// Control the baddie speeding up when angry (uses STATE_ANGRY)
// On Entry:
// 	slow_move = Normal Movement rate
//		fast_move = Fast Movement rate
// On Exit:
// 	0 = Normal Movement
//		Else object is angry
//------------------------------------------------------------------------------
int CLinkObject::ControlSpeedUp( OBJMOVE &slow_move, OBJMOVE &fast_move )
{
	if (m_State&STATE_ANGRY)
	{
		InitMovement(fast_move);
		return 1;
	}
	InitMovement(slow_move);
	return 0;
}

//------------------------------------------------------------------------------
// Check to see if this object has hit an object of a certain flag type
// On Entry:
//		base = Object list base to search for
// 	flag = FLAG_xxx bits to check for
// On Exit:
// 	0 = Not Hit
//		Else the object collided with this one
//------------------------------------------------------------------------------
CLinkObject *CLinkObject::CheckHitFlag( CObjectList &base, int flag )
{
	CLinkObject *pobj;

	for (pobj = base.m_pFirst; pobj; pobj = pobj->m_pNext)
	{
		if ((pobj->m_Flags & flag )==flag)
		{
			if (CheckHit(pobj))
			{
				return pobj;
			}
		}
	}
	return 0;
}


//******************************************************************************
// The Object List Connection Code :-
//******************************************************************************

//------------------------------------------------------------------------------
// Initialise the object list
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
CObjectList::CObjectList()
{
	m_pFirst = 0;
}

//------------------------------------------------------------------------------
// Destroy the object list
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
CObjectList::~CObjectList()
{
	DeleteAll();
}

//------------------------------------------------------------------------------
// Delete all the objects in the objectlist
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CObjectList::DeleteAll( void )
{
	CLinkObject *pobj;
	CLinkObject *pnext;

	pobj = m_pFirst;			// Start address
	while (pobj)			// For all objects
	{
		pnext = pobj->m_pNext;	// Get the next object
		delete pobj;		// Remove object
		pobj = pnext;
	}
	m_pFirst = 0;
}

//------------------------------------------------------------------------------
// Attach a object to the list (object will be linked in at the start)
// On Entry:
//		objptr = object to attach
// 	type = object type
//		gptr = Main Game Object
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CObjectList::Attach(CLinkObject *objptr, int type, CGame *gptr )
{
	if (objptr)
	{
		objptr->m_pGame = gptr;
		objptr->m_pListBase = this;
		objptr->m_Type = type;
		if (m_pFirst)	// Is this not the first object?
		{
			objptr->m_pNext = m_pFirst;	// Link in object
			m_pFirst->m_pPrev = objptr;
		}
		m_pFirst = objptr;
	}
}

//------------------------------------------------------------------------------
// Draw all the objects in the list
// Calls CLinkObject->Draw.
// DO NOT CLEAR m_Type
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CObjectList::DrawAll( void )
{

	CLinkObject *pobj;
	CLinkObject *pnext;

	pobj = m_pFirst;			// Start address
	while (pobj)			// For all objects
	{
		pnext = pobj->m_pNext;	// Get the next object
		if (OBJ_DEAD!=pobj->m_Type) pobj->Draw();		// Draw the object
		pobj = pnext;
	}
}

//------------------------------------------------------------------------------
// Do all the objects in the list
// Calls CLinkObject->Do.
// Clear m_Type to delete an object (the object will be automaticaly unlinked)
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CObjectList::DoAll( void )
{
	CLinkObject *pobj;
	CLinkObject *pnext;

	pobj = m_pFirst;						// Start address
	while (pobj)			// For all objects
	{
		pnext = pobj->m_pNext;	// Get the next object
		if (OBJ_DEAD!=pobj->m_Type) pobj->Do();		// Do the object
		if (OBJ_DEAD==pobj->m_Type)	// Object killed?
		{
			delete pobj;	// Remove object (unlink is called inside the destructor)
		}
		pobj = pnext;
	}
}

//------------------------------------------------------------------------------
// Find the first occurance of an object type
// On Entry:
// 	type = object to find
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
CLinkObject *CObjectList::FindFirst(int type)
{

	CLinkObject *pobj;

	pobj = m_pFirst;			// Start address
	while (pobj)			// For all objects
	{
		if (pobj->m_Type==type) return pobj;
		pobj = pobj->m_pNext;	// Get the next object
	}
	return 0;
}


