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

/*
 * GameCube Port by Infact <infact quantentunnel de> 10/2012
 */

//------------------------------------------------------------------------------
// Methane Brothers Document (Source File)
//------------------------------------------------------------------------------

#include <stdlib.h>

#include "SDL.h"
#include "doc.h"
#include "target.h"

//------------------------------------------------------------------------------
// The Screen, fixed video mode 320 x 256
//------------------------------------------------------------------------------
static char TheScreen[SCR_WIDTH * SCR_HEIGHT];

//------------------------------------------------------------------------------
// Custom video mode 320 x 240
//------------------------------------------------------------------------------
SDL_Surface* screen = 0;

//------------------------------------------------------------------------------
//! \brief Initialise Document
//------------------------------------------------------------------------------
CMethDoc::CMethDoc()
{
    SMB_NEW(m_pAudioDrv, CAudioDrv);

    m_GameTarget.Init(this);
    m_GameTarget.InitGame(TheScreen);
}

//------------------------------------------------------------------------------
//! \brief Destroy Document
//------------------------------------------------------------------------------
CMethDoc::~CMethDoc()
{
    if (m_pAudioDrv)
    {
        delete(m_pAudioDrv);
        m_pAudioDrv = 0;
    }
}

//------------------------------------------------------------------------------
//! \brief Initialise the game
//------------------------------------------------------------------------------
void CMethDoc::InitGame(void)
{
    m_GameTarget.InitGame(TheScreen);
    m_GameTarget.PrepareSoundDriver();
}

//------------------------------------------------------------------------------
//! \brief Initialise the sound driver
//------------------------------------------------------------------------------
void CMethDoc::InitSoundDriver(void)
{
    m_pAudioDrv->InitDriver();
}

//------------------------------------------------------------------------------
//! \brief Remove the sound driver
//------------------------------------------------------------------------------
void CMethDoc::RemoveSoundDriver(void)
{
    m_pAudioDrv->RemoveDriver();
}

//------------------------------------------------------------------------------
//! \brief Start the game
//------------------------------------------------------------------------------
void CMethDoc::StartGame(void)
{
    m_GameTarget.StartGame();
}

//------------------------------------------------------------------------------
//! \brief Redraw the game main view
//!
//! 	\param pal_change_flag : 0 = Palette not changed
//------------------------------------------------------------------------------
void CMethDoc::RedrawMainView( int pal_change_flag )
{
    // Function not used
}

//------------------------------------------------------------------------------
//! \brief Draw the screen
//!
//! 	\param screen_ptr = 32 bit per pixel screen
//!	\param paused_flag = 0 = Game not paused
//------------------------------------------------------------------------------
void CMethDoc::DrawScreen( void *screen_ptr, int paused_flag )
{
    METHANE_RGB *pal_ptr;
    int cnt;
    SDL_Color met_colors [PALETTE_SIZE];

    if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);

    // Set the game palette
    pal_ptr = m_GameTarget.m_rgbPalette;
    if (!paused_flag) {
        for (cnt=0; cnt < PALETTE_SIZE; cnt++, pal_ptr++) {
            met_colors[cnt].r = pal_ptr->red;
            met_colors[cnt].g = pal_ptr->green;
            met_colors[cnt].b = pal_ptr->blue;
        }
    } else { // If paused - greyscale the palette
        for (cnt=0; cnt < PALETTE_SIZE; cnt++, pal_ptr++) {
            int cval = (pal_ptr->red + pal_ptr->green + pal_ptr->blue) / 3;
            met_colors[cnt].r = cval;
            met_colors[cnt].g = cval;
            met_colors[cnt].b = cval;
        }
    }
    SDL_SetPalette(screen, SDL_LOGPAL|SDL_PHYSPAL, met_colors, 0, PALETTE_SIZE);

    // Copy the pixels
    char* dest_ptr = (char *) screen->pixels;
    char* src_ptr = TheScreen + 3 * 320; // Adjustment 256 -> 240 lines
    for (int y = 0; y < 240; y++) {
        for (int x = 0; x < 320; x++) {
            *dest_ptr++ = *src_ptr++;
        }
        // Only needed, if display width > 320px
        dest_ptr += (screen->pitch - 320);
    }

    if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);

    // Show the new screen
    SDL_Flip (screen);
}

//------------------------------------------------------------------------------
//! \brief The Game Main Loop 
//!
//! 	\param screen_ptr = 32 bit per pixel screen
//!	\param paused_flag = 0 = Game not paused
//------------------------------------------------------------------------------
void CMethDoc::MainLoop( void *screen_ptr, int paused_flag )
{
    if (!paused_flag)
        m_GameTarget.MainLoop();

    DrawScreen(screen_ptr, paused_flag);
}

//------------------------------------------------------------------------------
//! \brief Play a sample (called from the game)
//!
//! 	\param id = SND_xxx id
//!	\param pos = Sample Position to use 0 to 255
//!	\param rate = The rate
//------------------------------------------------------------------------------
void CMethDoc::PlaySample(int id, int pos, int rate)
{
    m_pAudioDrv->PlaySample(id, pos, rate);
}

//------------------------------------------------------------------------------
//! \brief Stop the module (called from the game)
//------------------------------------------------------------------------------
void CMethDoc::StopModule(void)
{
    m_pAudioDrv->StopModule();
}

//------------------------------------------------------------------------------
//! \brief Play a module (called from the game)
//!
//! 	\param id = SMOD_xxx id
//------------------------------------------------------------------------------
void CMethDoc::PlayModule(int id)
{
    m_pAudioDrv->PlayModule(id);
}

//------------------------------------------------------------------------------
//! \brief Update the current module (ie restart the module if it has stopped) (called from the game)
//!
//! 	\param id = SMOD_xxx id (The module that should be playing)
//------------------------------------------------------------------------------
void CMethDoc::UpdateModule(int id)
{
    // Nothing to do, MODPlay loops by itself
}

// Not implemented yet, needs function to insert name using controller first
//------------------------------------------------------------------------------
//! \brief Load the high scores
//------------------------------------------------------------------------------
void CMethDoc::LoadScores(void)
{

}

//------------------------------------------------------------------------------
//! \brief Save the high scores
//------------------------------------------------------------------------------
void CMethDoc::SaveScores(void)
{

}
