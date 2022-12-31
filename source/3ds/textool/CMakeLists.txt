cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)
project(smb-textool VERSION 1.0.0 LANGUAGES CXX)
add_executable(smb-textool
	main.cpp
	precomp.h
	precomp.cpp
	../source/global.h
	../source/gfxdef.h
	../source/gfxoff.cpp
)
target_include_directories(smb-textool PRIVATE
  .
  ../source
)
target_link_libraries(smb-textool PRIVATE
  freeimage
)
