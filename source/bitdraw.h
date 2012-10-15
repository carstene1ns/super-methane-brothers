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
// Methane Brothers Bitmap Draw (Header File)
//------------------------------------------------------------------------------

#ifndef _bitdraw_h
#define _bitdraw_h

class CBitmapDraw
{
private:
	char *CalcClip( int &xpos, int &ypos, int &width, int &height, int &xmod );
public:
	CBitmapDraw();
	~CBitmapDraw();
	int Load(int rid);
	void UnLoad(void);
	void Draw16( char *dest, int block_offset );
	void Draw(char *dest, int xpos, int ypos );
	void DrawWhite(char *dest, int xpos, int ypos );
	void DrawColour(char *dest, int xpos, int ypos );

	char	*m_pBitmap;
	short	m_Width;
	short	m_Height;
};

#endif // _bitdraw_h

