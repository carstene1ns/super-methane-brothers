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
// Methane Brothers Document (Source File)
//------------------------------------------------------------------------------
#include "doc.h"
#include "target.h"
#include "snddef.h"

//------------------------------------------------------------------------------
//! \brief Initialise Document
//!
//!	\param window = Screen to draw to
//------------------------------------------------------------------------------
CMethDoc::CMethDoc(CL_DisplayWindow &window)
{
	m_GameTarget.Init(this, window);
}

//------------------------------------------------------------------------------
//! \brief Destroy Document
//------------------------------------------------------------------------------
CMethDoc::~CMethDoc()
{
}

//------------------------------------------------------------------------------
//! \brief Initialise the game
//------------------------------------------------------------------------------
void CMethDoc::InitGame(void)
{
	m_GameTarget.InitGame();
	m_GameTarget.PrepareSoundDriver();
}

//------------------------------------------------------------------------------
//! \brief Start the game
//------------------------------------------------------------------------------
void CMethDoc::StartGame(void)
{
	m_GameTarget.StartGame();
}

//------------------------------------------------------------------------------
//! \brief The Game Main Loop 
//!
//! 	\param window = The screen
//------------------------------------------------------------------------------
void CMethDoc::MainLoop()
{
	m_GameTarget.MainLoop();
}

//------------------------------------------------------------------------------
//! \brief The Display Options screen
//!
//!	\param window = Current window
//!	\param page_no	= Instruction page number
//------------------------------------------------------------------------------
void CMethDoc::DisplayOptions( CL_DisplayWindow &window, CL_Font &font, int page_no )
{

	CL_GraphicContext gc = window.get_gc();

	if (page_no == 0)
	{
		font.draw_text(gc, 32, 2*32, "INSTRUCTIONS:");
		font.draw_text(gc, 32, 4*32, "FIRE TO START");
		font.draw_text(gc, 32, 5*32, "TYPE PLAYER NAMES");
		font.draw_text(gc, 32, 6*32, "TAP FIRE TO GAS");
		font.draw_text(gc, 32, 7*32, "HOLD FIRE TO SUCK");
		font.draw_text(gc, 32, 8*32, "RELEASE FIRE THROW");
		font.draw_text(gc, 32, 9*32, "THROW AT THE WALL");
	}
	if (page_no == 1)
	{
		font.draw_text(gc, 32, 2*32, "PLAYER ONE:");
		font.draw_text(gc, 32, 3*32, "USE CURSOR KEYS");
		font.draw_text(gc, 32, 4*32, "CTRL TO FIRE.");
		font.draw_text(gc, 32, 6*32, "PLAYER TWO:");
		font.draw_text(gc, 32, 7*32, "USE A W S D");
		font.draw_text(gc, 32, 8*32, "SHIFT TO FIRE");

	}
	if (page_no == 2)
	{
		font.draw_text(gc, 32, 2*32, "KEYS:");
		font.draw_text(gc, 32, 4*32, "F9 INCREASE SPEED");
		font.draw_text(gc, 32, 5*32, "F10 DECREASE SPEED");
		font.draw_text(gc, 32, 6*32, "F11 SKIP LEVEL");
		font.draw_text(gc, 32, 7*32, "TAB CHANGE GRAPHIC");
		font.draw_text(gc, 32, 8*32, "ESC EXIT PROGRAM");
	}

	font.draw_text(gc, 32, 12*32, "(PRESS SPACE)");

}

//------------------------------------------------------------------------------
//! \brief Load the high scores
//------------------------------------------------------------------------------
void CMethDoc::LoadScores(void)
{
	CL_String dirname = CL_Directory::get_appdata("clanlib", "methane", "1.5", false);

	try
	{
		CL_File file(dirname+"highscores");
		HISCORES *hs;
		int cnt;
		for (cnt=0, hs=m_GameTarget.m_Game.m_HiScores; cnt<MAX_HISCORES; cnt++, hs++)
		{
			char buffer[5];
			file.read(buffer, 4, true);
			buffer[4] = 0;
			int score = file.read_int32();

			m_GameTarget.m_Game.InsertHiScore( score, buffer );

		}
	}
	catch(CL_Exception& exception)
	{
	}

}

//------------------------------------------------------------------------------
//! \brief Save the high scores
//------------------------------------------------------------------------------
void CMethDoc::SaveScores(void)
{
	CL_String dirname = CL_Directory::get_appdata("clanlib", "methane", "1.5");

	try
	{
		CL_File file(dirname+"highscores", CL_File::create_always, CL_File::access_write);
		HISCORES *hs;
		int cnt;
		for (cnt=0, hs=m_GameTarget.m_Game.m_HiScores; cnt<MAX_HISCORES; cnt++, hs++)
		{
			file.write(hs->name, 4, true);
			file.write_int32(hs->score);
		}
	}
	catch(CL_Exception& exception)
	{
	}
}

