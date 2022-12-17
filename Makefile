METHANE_FLAGS = `pkg-config --cflags clanCore-4.1 clanDisplay-4.1 clanApp-4.1 clanGL-4.1 clanSound-4.1 libmikmod` -Isources
METHANE_LIBS = `pkg-config --libs clanCore-4.1 clanDisplay-4.1 clanApp-4.1 clanGL-4.1 clanSound-4.1 libmikmod`

OBJF = sources/precomp.o \
    sources/player.o \
    sources/sound.o \
    sources/gfxoff.o \
    sources/target.o \
    sources/bititem.o \
    sources/methane.o \
    sources/goodie.o \
    sources/objlist.o \
    sources/global.o \
    sources/gasobj.o \
    sources/game.o \
    sources/bitdraw.o \
    sources/baddie.o \
    sources/doc.o \
    sources/misc.o \
    sources/power.o \
    sources/bitgroup.o \
    sources/weapon.o \
    sources/render_batch_triangle.o \
    sources/ClanMikmod/soundprovider_mikmod.o \
    sources/ClanMikmod/module_reader.o \
    sources/ClanMikmod/setupmikmod.o \
    sources/ClanMikmod/soundprovider_mikmod_session.o \
    sources/suck.o \
    sources/mapdata.o \
    sources/boss.o \
    sources/maps.o

all: message methane

message:
	@echo "Compiling Super Methane Brothers."
	@echo "================================="

methane: ${OBJF}
	g++ ${CXXFLAGS} ${OBJF}	-o methane ${METHANE_LIBS}

clean:
	@rm -f sources/*.o
	@rm -f sources/ClanMikmod/*.o
	@rm -f methane

distclean: clean

# The main source code
%.o:%.cpp
	@echo "  Compiling $<..."
	gcc ${CXXFLAGS} ${METHANE_FLAGS} -c $< -o $@

