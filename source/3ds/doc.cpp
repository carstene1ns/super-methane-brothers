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
CMethDoc::CMethDoc()
{
	m_GameTarget.Init(this);
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
