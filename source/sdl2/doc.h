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
// Methane Brothers Document (Header File)
//------------------------------------------------------------------------------

#ifndef _doc_h
#define _doc_h 1

#include "game.h"
#include "target.h"

class CMethDoc
{

public:
	CMethDoc(SDL_Renderer* renderer, SDL_Surface* canvas, SDL_Texture* texture);
	~CMethDoc();

	void InitGame();
	void DeinitGame();
	void StartGame();
	void MainLoop();
	void DisplayOptions(SDL_Surface* canvas);

	void SaveScores();
	void LoadScores();

	CGameTarget	m_GameTarget;

};

#endif
