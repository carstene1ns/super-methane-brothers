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
// Methane Brothers 32 Colour Font (Header File)
//------------------------------------------------------------------------------

#ifndef _font32_h
#define _font32_h 1

#define FONT32_NUM_COLOURS	32	// The number of colours in the font
#define FONT32_NUM_IN_LIST 59	// Number of font charactors in the font32 list

// The font (0 = Letter not available).
// The letter has 4 control bytes: offset_x, offset_y, width, height
// The letter is 16*16 in size
// The letter data must only contain the maximum of FONT32_NUM_COLOURS (validation is not performed)
extern char *Spr_Font32[FONT32_NUM_IN_LIST];

extern int SMB_Font32_Palette[FONT32_NUM_COLOURS];

#endif

