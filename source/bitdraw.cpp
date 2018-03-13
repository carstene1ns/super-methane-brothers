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
// Methane Brothers Bitmap Draw (Source File)
//------------------------------------------------------------------------------

#include "global.h"
#include "bitdraw.h"
#include "gfxdef.h"

//------------------------------------------------------------------------------
// Constructor
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
CBitmapDraw::CBitmapDraw()
{
	m_pBitmap = 0;
}

//------------------------------------------------------------------------------
// Destructor
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
CBitmapDraw::~CBitmapDraw()
{
	UnLoad();
}


//------------------------------------------------------------------------------
// Load a graphic
// On Entry:
// 	rid = resource id
// On Exit:
// 	0 = okay, else error occured
//------------------------------------------------------------------------------
int CBitmapDraw::Load(int rid)
{
	unsigned char *xptr;
	MCOORDS *cptr;
	int offset;
	int bank1_size;
	int bank2_size;

	UnLoad();

	if ((rid < SPR_START_NUMBER) || (rid > SPR_END_NUMBER) ) return 0;

	cptr = &MainOffsets[rid - SPR_START_NUMBER];
	offset = cptr->datafile_offset;

	bank1_size = MainOffsets[MethaneGraphicData_Bank2_Sprite - SPR_START_NUMBER].datafile_offset;
	bank2_size = MainOffsets[MethaneGraphicData_Bank3_Sprite - SPR_START_NUMBER].datafile_offset - bank1_size;

	if (offset >= bank1_size)	// If graphic outside the first bank
	{
		offset = offset - bank1_size;
		if (offset >= bank2_size)	// If graphic outside the second bank
		{
			offset = offset - bank2_size;
			xptr = MethaneGraphicData_Bank3;
		}else
		{
			xptr = MethaneGraphicData_Bank2;
		}
	}else
	{

		xptr = MethaneGraphicData_Bank1;
	}
	xptr += offset;

	// Reverse the bytes
	m_Width = cptr->width;
	m_Height = cptr->height;
	m_pBitmap = (char *) xptr;

	return 0;
}

//------------------------------------------------------------------------------
// UnLoad a graphic
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CBitmapDraw::UnLoad(void)
{
	m_pBitmap = 0;
}

//------------------------------------------------------------------------------
// Draw a bitmap onto the screen 
// On Entry:
// 	dest = the screen address to draw to
//		xpos,ypos = The position to draw to
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CBitmapDraw::Draw(char *dest, int xpos, int ypos )
{
	int x;
	char colour;
	char *src;
	int width;
	int height;
	int xmod;

	src = CalcClip( xpos, ypos, width, height, xmod );
	if (src)
	{
		dest += xpos;
		dest += ypos * SCR_WIDTH;

		for (; height>0; height--)
		{
			for (x=0; x<width; x++)
			{
				colour = *(src++);
				if (colour) *(dest) = colour;
				dest++;
			}
			dest += SCR_WIDTH - width;
			src += xmod;
		}
	}
}

//------------------------------------------------------------------------------
// Draw a bitmap onto the screen with an alternative colour (different colour bank)
// On Entry:
// 	dest = the screen address to draw to
//		xpos,ypos = The position to draw to
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CBitmapDraw::DrawColour(char *dest, int xpos, int ypos )
{
	int x;
	char colour;
	char *src;
	int width;
	int height;
	int xmod;

	src = CalcClip( xpos, ypos, width, height, xmod );
	if (src)
	{
		dest += xpos;
		dest += ypos * SCR_WIDTH;

		for (; height>0; height--)
		{
			for (x=0; x<width; x++)
			{
				colour = *(src++);
				if (colour) *(dest) = colour << 2;
				dest++;
			}
			dest += SCR_WIDTH - width;
			src += xmod;
		}
	}
}


//------------------------------------------------------------------------------
// Draw a bitmap onto the screen - WHITE
// On Entry:
// 	dest = the screen address to draw to
//		xpos,ypos = The position to draw to
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CBitmapDraw::DrawWhite(char *dest, int xpos, int ypos )
{
	int x;
	char colour;
	char *src;
	int width;
	int height;
	int xmod;

	src = CalcClip( xpos, ypos, width, height, xmod );
	if (src)
	{
		dest += xpos;
		dest += ypos * SCR_WIDTH;

		for (; height>0; height--)
		{
			for (x=0; x<width; x++)
			{
				colour = *(src++);
				if (colour) *(dest) = 31;	// 31 = White
				dest++;
			}
			dest += SCR_WIDTH - width;
			src += xmod;
		}
	}
}


//------------------------------------------------------------------------------
// Draw a 16x16 section of a bitmap onto the screen
// Warning - NO validation is used
// On Entry:
// 	dest = the screen address to draw to
//		block_offset = the block id to draw
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CBitmapDraw::Draw16( char *dest, int block_offset )
{
	int y;
	char *src;
	src = m_pBitmap + (block_offset<<8);		// (*256 to get the correct graphic offset (16*16))
	for (y=0; y<16; y++)					// Draw the block
	{
		*(dest++) = *(src++);
		*(dest++) = *(src++);
		*(dest++) = *(src++);
		*(dest++) = *(src++);
		*(dest++) = *(src++);
		*(dest++) = *(src++);
		*(dest++) = *(src++);
		*(dest++) = *(src++);
		*(dest++) = *(src++);
		*(dest++) = *(src++);
		*(dest++) = *(src++);
		*(dest++) = *(src++);
		*(dest++) = *(src++);
		*(dest++) = *(src++);
		*(dest++) = *(src++);
		*(dest) = *(src++);
		dest += (SCR_WIDTH - 15);
	}
}

//------------------------------------------------------------------------------
// Calculate the clipping values when drawing a bitmap (Private Function)
// On Entry:
// 	xpos, ypos = Draw positions
//		width,height = Where to write the width and height
//		xmod = X source modulo
// On Exit:
// 	The source bitmap
//		0 = Do not draw
//------------------------------------------------------------------------------
char *CBitmapDraw::CalcClip( int &xpos, int &ypos, int &width, int &height, int &xmod )
{
	int diff;
	char *src;

	src = m_pBitmap;
	if (src)
	{
		width = m_Width;
		height = m_Height;

		if (ypos < 0)	// Off upper edge?
		{
			diff = -ypos ;
			height -= diff;
			src += diff * width;
			ypos = 0;
		}

		diff = (ypos + height) - SCR_HEIGHT;
		if (diff > 0)	// Off right edge?
		{
			height -= diff;
		}

		if (height<=0)	// Off screen?
		{
			return 0;
		}


		if (xpos < 0)	// Off left edge?
		{
			xmod = -xpos;		// X Modulo
			width -= xmod;
			src += xmod;
			xpos = 0;

		}else
		{
			xmod = 0;
		}
		diff = (xpos + width) - SCR_WIDTH;
		if (diff > 0)	// Off right edge?
		{
			xmod = xmod + diff;
			width -= diff;
		}

		if (width<=0)	// Off screen?
		{
			return 0;
		}

	}
	return src;
}


