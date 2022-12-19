# carstene1ns, 2017-2022

# reasonable defaults
cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 11)

# set build type to debug, if not set
if(DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
else()
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
endif()

project(super-methane-brothers VERSION 2.0.1 LANGUAGES CXX)

# be verbose
#set(CMAKE_VERBOSE_MAKEFILE on)

# for c++ files
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# pkg-config finds our needed libraries
include(FindPkgConfig)

set(CLANLIB 0)
set(SDL2 1)

# needed libraries
if(CLANLIB)
  pkg_check_modules(CL REQUIRED clanCore-4.1 clanDisplay-4.1 clanApp-4.1 clanGL-4.1 clanSound-4.1)
  set(LOC_INCS source/clanlib)
  set(EXT_INCS ${CL_INCLUDE_DIRS})
  set(EXT_LIBS ${CL_LIBRARIES})
elseif(SDL2)
  pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_image)
  set(LOC_INCS source/sdl2)
  set(EXT_INCS ${SDL2_INCLUDE_DIRS})
  set(EXT_LIBS ${SDL2_LIBRARIES})
endif()
pkg_check_modules(MIKMOD REQUIRED libmikmod)
set(EXT_INCS ${EXT_INCS} ${MIKMOD_INCLUDE_DIRS})
set(EXT_LIBS ${EXT_LIBS} ${MIKMOD_LIBRARIES})

# programs
add_executable(smb
  source/global.cpp
  source/mapdata.cpp
  source/maps.cpp
  source/misc.cpp
  source/boss.cpp
  source/baddie.cpp
  source/goodie.cpp
  source/bitdraw.cpp
  source/bitgroup.cpp
  source/bititem.cpp
  source/gasobj.cpp
  source/gfxoff.cpp
  source/objlist.cpp
  source/power.cpp
  source/sound.cpp
  source/suck.cpp
  source/weapon.cpp
  source/player.cpp
  source/game.cpp
)


if(CLANLIB)
  target_sources(smb PRIVATE
    source/clanlib/doc.cpp
    source/clanlib/target.cpp
    source/clanlib/precomp.cpp
    source/clanlib/render_batch_triangle.cpp
    source/clanlib/methane.cpp
    source/clanlib/ClanMikmod/module_reader.cpp
    source/clanlib/ClanMikmod/setupmikmod.cpp
    source/clanlib/ClanMikmod/soundprovider_mikmod.cpp
    source/clanlib/ClanMikmod/soundprovider_mikmod_session.cpp
  )
elseif(SDL2)
  target_sources(smb PRIVATE
    source/sdl2/doc.cpp
    source/sdl2/target.cpp
    source/sdl2/precomp.cpp
    source/sdl2/methane.cpp
  )
endif()

target_include_directories(smb PUBLIC
  source
  ${LOC_INCS}
  ${EXT_INCS}
)
target_link_libraries(smb
  ${EXT_LIBS}
)
