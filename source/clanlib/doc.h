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

#ifdef METHANE_MIKMOD
class CMikModDrv;
#endif

class CMethDoc
{

public:
	CMethDoc(clan::Canvas& canvas);
	~CMethDoc();

	void InitGame();
	void StartGame();
	void MainLoop();
	void DisplayOptions(clan::Canvas& canvas, clan::Font &font);

	void SaveScores();
	void LoadScores();

private:
#ifdef METHANE_MIKMOD
	CMikModDrv	*m_pMikModDrv;
#endif
public:
	CGameTarget	m_GameTarget;

};

#endif

