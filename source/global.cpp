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
// For global functions (Source File)
//------------------------------------------------------------------------------

#include "global.h"   

#ifndef METHANE_OLD_CPP_NEW
void * operator new(size_t size, char const* file, int line) throw(informative_bad_alloc)
{
	void* ptr = operator new(size, std::nothrow);
	if (!ptr) throw informative_bad_alloc(file, line);
	return ptr;
}

void * operator new[](size_t size, char const* file, int line) throw(informative_bad_alloc)
{
	void* ptr = operator new(size, std::nothrow);
	if (!ptr) throw informative_bad_alloc(file, line);
	return ptr;
}

#endif

//------------------------------------------------------------------------------
// (GLOBAL) Makes sure that a object coords are inside the screen
// On Entry:
//		xpos = Reference to the xpos
//		ypos = Reference to the ypos
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CheckPos(int &xpos, int &ypos)
{
	if (xpos<0) xpos+=SCR_WIDTH;	// Check offsets to put sprite on screen
	if (ypos<0) ypos+=SCR_HEIGHT;
	if (xpos>=SCR_WIDTH) xpos-=SCR_WIDTH;
	if (ypos>=SCR_HEIGHT) ypos-=SCR_HEIGHT;
}

