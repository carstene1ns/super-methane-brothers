
# Comment out the two lines shown below to disable sound
MIKMOD_LIBS = `libmikmod-config --cflags` `libmikmod-config --libs`
METHANE_FLAGS = -DMETHANE_MIKMOD

all: message methane

# Methane brothers with sound
message:
	@echo "Compiling Super Methane Brothers."
	@echo "================================="
	@echo Using Flags: ${METHANE_FLAGS}

# Basic methane brothers compilation
methane: MainSource/gfxoff.o MainSource/baddie.o MainSource/bitdraw.o MainSource/bitgroup.o MainSource/bititem.o \
	MainSource/boss.o MainSource/game.o MainSource/gasobj.o MainSource/global.o MainSource/goodie.o MainSource/maps.o \
	MainSource/misc.o MainSource/objlist.o MainSource/player.o MainSource/power.o MainSource/target.o \
	MainSource/suck.o MainSource/weapon.o MainSource/sound.o MainData/snddata.o MainData/gfxdata.o MainData/gfxdata2.o \
	MainData/gfxdata3.o MainData/mapdata.o MainLinux/font32.o MainLinux/doc.o MainLinux/main.o MainMikMod/audiodrv.o
	g++ ${CXXFLAGS} MainSource/gfxoff.o MainSource/baddie.o MainSource/bitdraw.o MainSource/bitgroup.o MainSource/bititem.o \
	MainSource/boss.o MainSource/game.o MainSource/gasobj.o MainSource/global.o MainSource/goodie.o MainSource/maps.o \
	MainSource/misc.o MainSource/objlist.o MainSource/player.o MainSource/power.o MainSource/target.o \
	MainSource/suck.o MainSource/weapon.o MainSource/sound.o MainData/snddata.o MainData/gfxdata.o MainData/gfxdata2.o \
	MainData/gfxdata3.o MainData/mapdata.o MainLinux/font32.o MainLinux/doc.o MainLinux/main.o MainMikMod/audiodrv.o \
	-o methane -lclanCore -lclanApp -lclanDisplay ${MIKMOD_LIBS}

clean:
	@rm -Rf MainSource
	@rm -Rf MainData
	@rm -Rf MainLinux
	@rm -Rf MainMikMod
	@rm -f methane

distclean: clean


# The main source code
MainSource/%.o : ../%.cpp
	@echo "  Compiling $<..."
	@if [ ! -d MainSource ]; then mkdir MainSource; fi
	gcc ${CXXFLAGS} ${METHANE_FLAGS} -I ../linux `libmikmod-config --cflags` -c $< -o $@

# The main data
MainData/%.o : ../data/%.cpp
	@echo "  Compiling $<..."
	@if [ ! -d MainData ]; then mkdir MainData; fi
	gcc ${CXXFLAGS} ${METHANE_FLAGS} -I ../ `libmikmod-config --cflags` -c $< -o $@

# The linux source code
MainLinux/%.o : ../linux/%.cpp
	@echo "  Compiling $<..."
	@if [ ! -d MainLinux ]; then mkdir MainLinux; fi
	gcc ${CXXFLAGS} ${METHANE_FLAGS} -I ../ `libmikmod-config --cflags` -c $< -o $@

# The sound driver source code
MainMikMod/%.o : ../mikmod/%.cpp
	@echo "  Compiling $<..."
	@if [ ! -d MainMikMod ]; then mkdir MainMikMod; fi
	gcc ${CXXFLAGS} ${METHANE_FLAGS} -I ../ `libmikmod-config --cflags` -c $< -o $@

