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
// Methane Brothers Document (Source File)
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

#include "doc.h"
#include "target.h"
#include "snddef.h"
#include "font32.h"

#ifdef METHANE_MIKMOD
#include "../mikmod/audiodrv.h"
#endif

//------------------------------------------------------------------------------
// The Screen
//------------------------------------------------------------------------------
static char TheScreen[SCR_WIDTH * SCR_HEIGHT];

//------------------------------------------------------------------------------
// The HighScore table filename
//------------------------------------------------------------------------------
static char HighScoreFileName[] = "/var/games/methanescores";
#define HighScoreLoadBufferSize (MAX_HISCORES * 64)

//------------------------------------------------------------------------------
// Initialise Document
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
CMethDoc::CMethDoc()
{
#ifdef METHANE_MIKMOD
	SMB_NEW(m_pMikModDrv,CMikModDrv);
#endif
	m_GameTarget.Init(this);

	m_GameTarget.InitGame(TheScreen);
}

//------------------------------------------------------------------------------
// Destroy Document
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
CMethDoc::~CMethDoc()
{
#ifdef METHANE_MIKMOD
	if (m_pMikModDrv)
	{
		delete(m_pMikModDrv);
		m_pMikModDrv = 0;
	}
#endif
}

//------------------------------------------------------------------------------
// Initialise the game
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::InitGame(void)
{
	m_GameTarget.InitGame(TheScreen);
	m_GameTarget.PrepareSoundDriver();
}

//------------------------------------------------------------------------------
// Initialise the sound driver
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::InitSoundDriver(void)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->InitDriver();
#endif
}

//------------------------------------------------------------------------------
// Remove the sound driver
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::RemoveSoundDriver(void)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->RemoveDriver();
#endif
}


//------------------------------------------------------------------------------
// Start the game
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::StartGame(void)
{
	m_GameTarget.StartGame();
}

//------------------------------------------------------------------------------
// Redraw the game main view
// On Entry:
// 	pal_change_flag : 0 = Palette not changed
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::RedrawMainView( int pal_change_flag )
{
	// Function not used
}

//------------------------------------------------------------------------------
// Draw the screen
// On Entry:
// 	screen_ptr = 32 bit per pixel screen
//	paused_flag : 0 = Game not paused
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::DrawScreen( void *screen_ptr, int paused_flag )
{
	int cnt;
	int *dptr;
	char *sptr;
	int colour;
	METHANE_RGB *pptr;
	METHANE_RGB *mptr;
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	int *tptr_base;
	int *tptr;

	// Allocate a temporary palette to assist conversion
	tptr_base = (int *) malloc( sizeof(int) * PALETTE_SIZE );
	if (!tptr_base) return;

	// Convert the game palette to 32bit RGB
	pptr = m_GameTarget.m_rgbPalette;
	tptr = tptr_base;

	if (!paused_flag)
	{
		for (cnt=0; cnt < PALETTE_SIZE; cnt++, pptr++)
		{
			mptr = pptr;
			red = mptr->red;
			green = mptr->green;
			blue = mptr->blue; 
			colour = (red << 24) | (green << 16) | (blue << 8);
			*(tptr++) = colour;
		}

	}else		// If paused - grey scale the palette
	{
		for (cnt=0; cnt < PALETTE_SIZE; cnt++, pptr++)
		{
			mptr = pptr;
			red = mptr->red;
			green = mptr->green;
			blue = mptr->blue;
			int cval = (red + green + blue) / 3;
			red = cval;
			colour = (red << 24) | (red << 16) | (red << 8);
			*(tptr++) = colour;
		}
	}


	// Write to the screen
	dptr = (int *) screen_ptr;
	sptr = TheScreen;
	tptr = tptr_base;
	for (cnt = 0; cnt < (SCR_WIDTH*SCR_HEIGHT); cnt++)
	{
		*(dptr++) = tptr[*(sptr++)];
	}

	// Deallocate the temporary palette
	free(tptr_base);

}

//------------------------------------------------------------------------------
// The Game Main Loop 
// On Entry:
// 	screen_ptr = 32 bit per pixel screen
//	paused_flag : 0 = Game not paused
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::MainLoop( void *screen_ptr, int paused_flag )
{
	if (!paused_flag)
	{
		m_GameTarget.MainLoop();
	}
	DrawScreen( screen_ptr, paused_flag );

#ifdef METHANE_MIKMOD
	m_pMikModDrv->Update();
#endif
}

//------------------------------------------------------------------------------
// Play a sample (called from the game)
// On Entry:
// 	id = SND_xxx id
//	pos = Sample Position to use 0 to 255
//	rate = The rate
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::PlaySample(int id, int pos, int rate)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->PlaySample(id, pos, rate);
#endif
}

//------------------------------------------------------------------------------
// Stop the module (called from the game)
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::StopModule(void)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->StopModule();
#endif
}

//------------------------------------------------------------------------------
// Play a module (called from the game)
// On Entry:
// 	id = SMOD_xxx id
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::PlayModule(int id)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->PlayModule(id);
#endif
}

//------------------------------------------------------------------------------
// Update the current module (ie restart the module if it has stopped) (called from the game)
// On Entry:
// 	id = SMOD_xxx id (The module that should be playing)
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::UpdateModule(int id)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->UpdateModule(id);
#endif
}

//------------------------------------------------------------------------------
// The Display Options screen
// On Entry:
// 	screen_ptr = 32 bit per pixel screen
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::DisplayOptions( void *screen_ptr )
{
	int *dptr;
	dptr = (int *) screen_ptr;

	DrawFont32Text( dptr, 0*16, "INSTRUCTIONS:");
	DrawFont32Text( dptr, 2*16, "CTRL:START GAME");
	DrawFont32Text( dptr, 3*16, "CTRL:CREATE/SUCK GAS");
	DrawFont32Text( dptr, 4*16, "CURSOR:MOVE PLAYER");
	DrawFont32Text( dptr, 5*16, "TAB:CHANGE GRAPHIC");
	DrawFont32Text( dptr, 6*16, "ESCAPE:EXIT PROGRAM");
	DrawFont32Text( dptr, 7*16, "F1:PAUSE");
	DrawFont32Text( dptr, 8*16, "F2:TOGGLE SCALING");
	DrawFont32Text( dptr, 9*16, "F3:FULL SCREEN");
	DrawFont32Text( dptr, 10*16, "F4:800'600 MODE");
	DrawFont32Text( dptr, 11*16, "F5:1024'768 MODE");
	DrawFont32Text( dptr, 12*16, "F6:1152'870 MODE");
	DrawFont32Text( dptr, 13*16, "F9,F10:GAME SPEED");

	DrawFont32Text( dptr, 15*16, "(PRESS SPACE)");

}
//------------------------------------------------------------------------------
// Draw the 32 colour font text (for the options screen)
// On Entry:
// 	dptr = 32 bit per pixel screen
//	ypos = Font y pos in pixels
//	tptr = The text
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::DrawFont32Text( int *dptr, int ypos, char *tptr)
{
	int txtlen;
	int xpos;
	int *wptr;
	char letter;
	char *fptr;
	int width;
	int height;
	int offset;
	int cnt;
	int *palette;

	txtlen = ::strlen(tptr);
	
	//xpos = (SCR_WIDTH/2) - (txtlen*(16/2));
	xpos = 0;
	ypos = ypos * SCR_WIDTH;

	dptr = dptr + xpos + ypos;

	palette = SMB_Font32_Palette;
	for (; txtlen>0; txtlen--)
	{
		letter = *(tptr++);
		// Validate the letter
		if ( (letter <32) || (letter > (FONT32_NUM_IN_LIST+32)) ) break;
		letter -=32;
		fptr = Spr_Font32[letter];
		if (fptr)	// Valid font
		{
			wptr = dptr;	// Save the pointer

			// Get sizes
			xpos = *(fptr++);
			ypos = *(fptr++);
			width = *(fptr++);
			height = *(fptr++);

			ypos = ypos * SCR_WIDTH;
			wptr = wptr + xpos + ypos;	// To the correct position

			offset = SCR_WIDTH - width;	// Offset to next line on the screen

			// For all rows in the font
			for (; height > 0; height--)
			{
				for (cnt=0; cnt<width; cnt++)
				{
					*(wptr++) = palette[( *(fptr++) )];
				}
				wptr+= offset;
			}

		}
		dptr += 16;	// To the next font position
	}

}

//------------------------------------------------------------------------------
// Load the high scores
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::LoadScores(void)
{
	FILE *fptr;
	char *mptr;
	char *tptr;
	char let;
	int cnt;

	fptr = fopen(HighScoreFileName, "r");
	if (!fptr) return;	// No scores available

	// Allocate file memory, which is cleared to zero
	mptr = (char *) calloc(1, HighScoreLoadBufferSize);
	if (!mptr)		// No memory
	{
		fclose(fptr);
		return;
	}
	fread( mptr, 1, HighScoreLoadBufferSize-2, fptr);	// Get the file

	// (Note: mptr is zero terminated)
	tptr = mptr;
	for (cnt=0; cnt<MAX_HISCORES; cnt++)	// For each highscore
	{

		if (!tptr[0]) break;

		m_GameTarget.m_Game.InsertHiScore( atoi(&tptr[4]), tptr );

		do	// Find next name
		{
			let = *(tptr++);
		}while (!( (let=='$') || (!let) ));
		if (!let) break;	// Unexpected EOF
	}

	free(mptr);

	fclose(fptr);

}

//------------------------------------------------------------------------------
// Save the high scores
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CMethDoc::SaveScores(void)
{
	FILE *fptr;
	int cnt;
	HISCORES *hs;

	fptr = fopen(HighScoreFileName, "w");
	if (!fptr) return;	// Cannot write scores
	for (cnt=0, hs=m_GameTarget.m_Game.m_HiScores; cnt<MAX_HISCORES; cnt++, hs++)
	{
		fprintf(fptr, "%c%c%c%c%d$", hs->name[0], hs->name[1], hs->name[2], hs->name[3], hs->score);
	}
	fclose(fptr);

}

