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

# platform
set(3DS 0)
set(SDL2 0)
set(CLANLIB 0)
set(SMB_TARGET "SDL2" CACHE STRING "Platform to compile for. Options: SDL2, 3DS, CLANLIB")
set_property(CACHE SMB_TARGET PROPERTY STRINGS SDL2 3DS CLANLIB)
if(SMB_TARGET STREQUAL "3DS")
  set(3DS 1)
elseif(SMB_TARGET STREQUAL "SDL2")
  set(SDL2 1)
elseif(SMB_TARGET STREQUAL "CLANLIB")
  set(CLANLIB 1)
else()
  message(FATAL_ERROR "Please select a valid platform: 3DS, SDL2, CLANLIB")
endif()

# programs
add_executable(smb
  source/baddie.cpp
  source/bitdraw.cpp
  source/bitgroup.cpp
  source/bititem.cpp
  source/boss.cpp
  source/game.cpp
  source/gasobj.cpp
  source/gfxoff.cpp
  source/global.cpp
  source/goodie.cpp
  source/mapdata.cpp
  source/maps.cpp
  source/misc.cpp
  source/objlist.cpp
  source/player.cpp
  source/power.cpp
  source/sound.cpp
  source/suck.cpp
  source/weapon.cpp
)

# resources
set(SMB_GRAPHICS
  resources/graphics/page_01.png
  resources/graphics/page_02.png
  resources/graphics/page_03.png
  resources/graphics/page_04.png
  resources/graphics/page_05.png
)
set(SMB_MUSIC
  resources/music/boss.mod
  resources/music/complete.mod
  resources/music/empty.mod
  resources/music/title.mod
  resources/music/tune1.mod
  resources/music/tune2.mod
)
set(SMB_SOUNDS
  resources/sounds/blow.wav
  resources/sounds/bowling.wav
  resources/sounds/candle.wav
  resources/sounds/car.wav
  resources/sounds/card.wav
  resources/sounds/chicken.wav
  resources/sounds/cookie.wav
  resources/sounds/crying.wav
  resources/sounds/day.wav
  resources/sounds/die2.wav
  resources/sounds/duck.wav
  resources/sounds/feather.wav
  resources/sounds/finlev1.wav
  resources/sounds/hurry.wav
  resources/sounds/marble.wav
  resources/sounds/mask.wav
  resources/sounds/moon.wav
  resources/sounds/oil.wav
  resources/sounds/pickup1.wav
  resources/sounds/pstar.wav
  resources/sounds/redstar.wav
  resources/sounds/spiningtop.wav
  resources/sounds/spit.wav
  resources/sounds/splat.wav
  resources/sounds/tap.wav
  resources/sounds/train.wav
  resources/sounds/tribble.wav
  resources/sounds/turbo.wav
  resources/sounds/twinkle.wav
  resources/sounds/wings.wav
  resources/sounds/wpotion.wav
  resources/sounds/xylo.wav
)

# needed libraries
include(FindPkgConfig)
if(3DS)
  set(LOC_INCS source/3ds)
  set(EXT_LIBS -lcitro2d -lcitro3d)
elseif(CLANLIB)
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
elseif(3DS)
  target_sources(smb PRIVATE
    source/3ds/doc.cpp
    source/3ds/target.cpp
    source/3ds/precomp.cpp
    source/3ds/methane.cpp
  )

  enable_language(ASM)

  # use raw sounds
  list(TRANSFORM SMB_SOUNDS REPLACE "/sounds/" "\\0raw/")
  list(TRANSFORM SMB_SOUNDS REPLACE "\\.wav$" ".raw")

  dkp_add_embedded_binary_library(3ds-assets
    resources/graphics/3ds/blocks.t3x
    resources/graphics/3ds/sprites.t3x
    ${SMB_MUSIC}
    ${SMB_SOUNDS}
  )
  target_link_libraries(smb PRIVATE 3ds-assets)

  ctr_generate_smdh(OUTPUT smb.smdh
    NAME "Super Methane Brothers"
    DESCRIPTION "Platformer with a gas gun"
    AUTHOR "carstene1ns, rombust"
    #ICON ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/3ds.png
  )
  ctr_create_3dsx(smb SMDH smb.smdh)
endif()

target_include_directories(smb PRIVATE
  source
  ${LOC_INCS}
  ${EXT_INCS}
)
target_link_libraries(smb PRIVATE
  ${EXT_LIBS}
)
