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

class CL_DisplayWindow;

class CMethDoc
{

public:
	CMethDoc(CL_DisplayWindow &window);
	~CMethDoc();

	void InitGame(void);
	void StartGame(void);
	void MainLoop();
	void DisplayOptions( CL_DisplayWindow &window, CL_Font &font, int page_no );

	void SaveScores(void);
	void LoadScores(void);

private:
#ifdef METHANE_MIKMOD
	CMikModDrv	*m_pMikModDrv;
#endif
public:
	CGameTarget	m_GameTarget;

};

#endif

