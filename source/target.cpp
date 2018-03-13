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
// GameTarget class. Contains a wrapper to connect the game to the OS (Source File)
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include "target.h"
#include "doc.h"

//------------------------------------------------------------------------------
// The game target (Yuck global!)
// Thus - Only a single GameTarget is allowed
//------------------------------------------------------------------------------
CGameTarget *GLOBAL_GameTarget = 0;

//------------------------------------------------------------------------------
// Constructor
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
CGameTarget::CGameTarget()
{
	// Clear the joystick!
	memset(&m_Joy1, 0, sizeof(m_Joy1));
	memset(&m_Joy2, 0, sizeof(m_Joy2));

	// Clear the palette
	memset(m_rgbPalette, 0, sizeof(m_rgbPalette));

	m_FadeChangeFlag = 0;	// Palete has not changed

	GLOBAL_GameTarget = this;	// The global target
}

//------------------------------------------------------------------------------
// Initialisation
// On Entry:
// 	pdoc = Pointer to the document this target belongs to
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CGameTarget::Init(CMethDoc *pdoc)
{
	m_pDoc = pdoc;
}

//------------------------------------------------------------------------------
// Initialise the game
// On Entry:
//		sptr = The main screen bitmap (SCR_WIDTH * SCR_HEIGHT 256 colour)
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CGameTarget::InitGame(char *sptr)
{
	m_Game.Init(this, sptr, &m_Joy1, &m_Joy2);
}

//------------------------------------------------------------------------------
// Redraw screen (Called by the game)
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CGameTarget::RedrawScreen(void)
{
	m_pDoc->RedrawMainView(m_FadeChangeFlag);
}

//------------------------------------------------------------------------------
// Start the game
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CGameTarget::StartGame(void)
{
	m_Game.StartGame();
}

//------------------------------------------------------------------------------
// Do the game main loop (Call every cycle)
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CGameTarget::MainLoop(void)
{
	m_Game.MainLoop();
}

//------------------------------------------------------------------------------
// Prepare the sound driver (call before the game starts)
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CGameTarget::PrepareSoundDriver(void)
{
	m_Game.m_Sound.PrepareAudio();
}

//------------------------------------------------------------------------------
// Play a module (called from the game)
// On Entry:
// 	id = SMOD_xxx id
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CGameTarget::PlayModule(int id)
{
	m_pDoc->PlayModule(id);
}
//------------------------------------------------------------------------------
// Stop the module (called from the game)
// On Entry:
// 	Not Used
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CGameTarget::StopModule(void)
{
	m_pDoc->StopModule();
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
void CGameTarget::PlaySample(int id, int pos, int rate)
{
	m_pDoc->PlaySample(id, pos, rate);
}

//------------------------------------------------------------------------------
// Update the current module (ie restart the module if it has stopped)
// On Entry:
// 	id = SMOD_xxx id (The module that should be playing)
// On Exit:
// 	Not Used
//------------------------------------------------------------------------------
void CGameTarget::UpdateModule(int id)
{
	m_pDoc->UpdateModule(id);

}

