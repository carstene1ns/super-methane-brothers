#pragma once

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctype.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <vector>

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/sound.h>

//#define DISABLE_SOUND
#ifndef DISABLE_SOUND
#include <mikmod.h>
#endif
