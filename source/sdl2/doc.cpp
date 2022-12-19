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
#include "precomp.h"
#include "doc.h"
#include "target.h"
#include "snddef.h"

//------------------------------------------------------------------------------
//! \brief Initialise Document
//!
//!	\param canvas = Screen to draw to
//------------------------------------------------------------------------------
CMethDoc::CMethDoc(SDL_Renderer *renderer, SDL_Surface *canvas, SDL_Texture *texture)
{
	m_GameTarget.Init(this, renderer, canvas, texture);
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
void CMethDoc::InitGame()
{
	m_GameTarget.InitGame();
	m_GameTarget.PrepareSoundDriver();
}

void CMethDoc::DeinitGame()
{
	m_GameTarget.FreeSoundDriver();
	m_GameTarget.DeinitGame();
}

//------------------------------------------------------------------------------
//! \brief Start the game
//------------------------------------------------------------------------------
void CMethDoc::StartGame()
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
//!	\param canvas = Current Canvas
//------------------------------------------------------------------------------
void CMethDoc::DisplayOptions(SDL_Surface *canvas)
{
#if 0
	clan::SpanLayout layout;
	layout.add_text("Player One: Use Cursor Keys. CTRL to fire\n", font, clan::Colorf::white);
	layout.add_text("Player Two: Use A,W,S,D. SHIFT to fire\n \n", font, clan::Colorf::white);
	layout.add_text("TAB - Toggles Player graphics\n", font, clan::Colorf::white);
	layout.add_text("ESC - Exit game\n \n", font, clan::Colorf::white);
	layout.add_text("Game - Fire to Start. Type Player Names. Tap Fire to Gas. Hold Fire to Suck. Release Fire Throw. Throw at the wall.\n \n", font, clan::Colorf::white);
	layout.add_text("Press any key to continue", font, clan::Colorf::white);
	layout.layout(canvas, canvas.get_width());
	layout.draw_layout(canvas);
#endif
}

//------------------------------------------------------------------------------
//! \brief Load the high scores
//------------------------------------------------------------------------------
void CMethDoc::LoadScores()
{

}

//------------------------------------------------------------------------------
//! \brief Save the high scores
//------------------------------------------------------------------------------
void CMethDoc::SaveScores()
{

}
