# Project:   SMB


# Toolflags:
CCflags = -c -depend !Depend -IC: -Otime -Ospace -throwback
C++flags = -c -faf -depend !Depend -IC: -Otime -Ospace -throwback -DMETHANE_RISCOS -DMETHANE_OLD_CPP_NEW -DMETHANE_OLD_CPP
Linkflags = -aif -c++ -o $@
ObjAsmflags = -throwback -NoCache -depend !Depend
CMHGflags =
LibFileflags = -c -o $@
Squeezeflags = -o $@

Libs = C:o.stubs c:o.c++lib

# Final targets:
@.!Runimage: $(Libs) @.o.doc @.o.swicalls @.o.gfxoff @.o.baddie @.o.bitdraw \
           @.o.bitgroup @.o.bititem @.o.boss @.o.game @.o.gasobj @.o.global \
           @.o.goodie @.o.maps @.o.misc @.o.objlist @.o.player @.o.power \
           @.o.power @.o.target @.o.suck @.o.weapon @.o.sound @.o.snddata \
           @.o.gfxdata @.o.gfxdata2 @.o.gfxdata3 @.o.mapdata
             Link $(Linkflags) \
               @.o.doc @.o.swicalls @.o.gfxoff @.o.baddie @.o.bitdraw \
           @.o.bitgroup @.o.bititem @.o.boss @.o.game @.o.gasobj @.o.global \
           @.o.goodie @.o.maps @.o.misc @.o.objlist @.o.player \
           @.o.power @.o.target @.o.suck @.o.weapon @.o.sound @.o.snddata \
           @.o.gfxdata @.o.gfxdata2 @.o.gfxdata3 @.o.mapdata $(Libs)

squeeze: !Runimage

# Static Dependancies:

@.o.doc:    @.c++.doc
        c++ $(c++flags) -o @.o.doc @.c++.doc
@.o.swicalls:    @.c++.swicalls
        c++ $(c++flags) -o @.o.swicalls @.c++.swicalls
@.o.gfxoff:    @.c++.gfxoff
        c++ $(c++flags) -o @.o.gfxoff @.c++.gfxoff
@.o.baddie:     @.c++.baddie
        c++ $(c++flags) -o @.o.baddie @.c++.baddie
@.o.bitdraw:	@.c++.bitdraw
	c++ $(c++flags) -o @.o.bitdraw @.c++.bitdraw
@.o.bitgroup:	@.c++.bitgroup
	c++ $(c++flags) -o @.o.bitgroup @.c++.bitgroup
@.o.bititem:	@.c++.bititem
	c++ $(c++flags) -o @.o.bititem @.c++.bititem
@.o.boss:	@.c++.boss
	c++ $(c++flags) -o @.o.boss @.c++.boss
@.o.game:	@.c++.game
	c++ $(c++flags) -o @.o.game @.c++.game
@.o.gasobj:	@.c++.gasobj
	c++ $(c++flags) -o @.o.gasobj @.c++.gasobj
@.o.global:	@.c++.global
	c++ $(c++flags) -o @.o.global @.c++.global
@.o.goodie:	@.c++.goodie
	c++ $(c++flags) -o @.o.goodie @.c++.goodie
@.o.maps:	@.c++.maps
	c++ $(c++flags) -o @.o.maps @.c++.maps
@.o.misc:	@.c++.misc
	c++ $(c++flags) -o @.o.misc @.c++.misc
@.o.objlist:	@.c++.objlist
	c++ $(c++flags) -o @.o.objlist @.c++.objlist
@.o.player:	@.c++.player
	c++ $(c++flags) -o @.o.player @.c++.player
@.o.power:	@.c++.power
	c++ $(c++flags) -o @.o.power @.c++.power
@.o.target:	@.c++.target
	c++ $(c++flags) -o @.o.target @.c++.target
@.o.suck:	@.c++.suck
	c++ $(c++flags) -o @.o.suck @.c++.suck
@.o.weapon:	@.c++.weapon
	c++ $(c++flags) -o @.o.weapon @.c++.weapon
@.o.sound:	@.c++.sound
	c++ $(c++flags) -o @.o.sound @.c++.sound
@.o.snddata:	@.c++.snddata
	c++ $(c++flags) -o @.o.snddata @.c++.snddata
@.o.gfxdata:	@.c++.gfxdata
	c++ $(c++flags) -o @.o.gfxdata @.c++.gfxdata
@.o.gfxdata2:	@.c++.gfxdata2
	c++ $(c++flags) -o @.o.gfxdata2 @.c++.gfxdata2
@.o.gfxdata3:	@.c++.gfxdata3
	c++ $(c++flags) -o @.o.gfxdata3 @.c++.gfxdata3
@.o.mapdata:	@.c++.mapdata
	c++ $(c++flags) -o @.o.mapdata @.c++.mapdata

# Dynamic dependencies:
o.gfxdata2:	c++.gfxdata2
o.gfxdata2:	h.gfxdef
o.gfxdata2:	h.global
o.gfxdata3:	c++.gfxdata3
o.gfxdata3:	h.gfxdef
o.gfxdata3:	h.global
o.mapdata:	c++.mapdata
o.mapdata:	h.global
o.mapdata:	h.mapdef
o.gfxoff:	c++.gfxoff
o.gfxoff:	h.global
o.baddie:	c++.baddie
o.baddie:	h.baddie
o.baddie:	h.objlist
o.baddie:	h.suck
o.baddie:	h.objlist
o.baddie:	h.global
o.baddie:	h.bitgroup
o.baddie:	h.gfxdef
o.baddie:	h.global
o.baddie:	h.player
o.baddie:	h.objlist
o.baddie:	h.objtypes
o.baddie:	h.game
o.baddie:	h.maps
o.baddie:	h.global
o.baddie:	h.bitdraw
o.baddie:	h.bititem
o.baddie:	h.bitgroup
o.baddie:	h.objlist
o.baddie:	h.sound
o.baddie:	h.snddef
o.baddie:	h.global
o.baddie:	h.weapon
o.baddie:	h.objlist
o.baddie:	h.goodie
o.baddie:	h.objlist
o.baddie:	h.suck
o.baddie:	h.maps
o.bitdraw:	c++.bitdraw
o.bitdraw:	h.global
o.bitdraw:	h.bitdraw
o.bitdraw:	h.gfxdef
o.bitdraw:	h.global
o.bitgroup:	c++.bitgroup
o.bitgroup:	h.bitgroup
o.bitgroup:	h.gfxdef
o.bitgroup:	h.global
o.bitgroup:	h.global
o.bitgroup:	h.bititem
o.bitgroup:	h.game
o.bitgroup:	h.maps
o.bitgroup:	h.global
o.bitgroup:	h.bitdraw
o.bitgroup:	h.bititem
o.bitgroup:	h.bitgroup
o.bitgroup:	h.objlist
o.bitgroup:	h.sound
o.bitgroup:	h.snddef
o.bitgroup:	h.global
o.bititem:	c++.bititem
o.bititem:	h.bitdraw
o.bititem:	h.bititem
o.bititem:	h.global
o.bititem:	h.maps
o.bititem:	h.global
o.bititem:	h.bitdraw
o.bititem:	h.game
o.bititem:	h.maps
o.bititem:	h.bititem
o.bititem:	h.bitgroup
o.bititem:	h.gfxdef
o.bititem:	h.global
o.bititem:	h.objlist
o.bititem:	h.sound
o.bititem:	h.snddef
o.bititem:	h.global
o.boss:	c++.boss
o.boss:	h.boss
o.boss:	h.objlist
o.boss:	h.suck
o.boss:	h.objlist
o.boss:	h.global
o.boss:	h.bitgroup
o.boss:	h.gfxdef
o.boss:	h.global
o.boss:	h.objtypes
o.boss:	h.goodie
o.boss:	h.objlist
o.boss:	h.suck
o.boss:	h.maps
o.boss:	h.global
o.boss:	h.bitdraw
o.boss:	h.game
o.boss:	h.maps
o.boss:	h.bititem
o.boss:	h.bitgroup
o.boss:	h.objlist
o.boss:	h.sound
o.boss:	h.snddef
o.boss:	h.global
o.boss:	h.misc
o.boss:	h.objlist
o.boss:	h.maps
o.boss:	h.weapon
o.boss:	h.objlist
o.boss:	h.baddie
o.boss:	h.objlist
o.boss:	h.suck
o.game:	c++.game
o.game:	h.global
o.game:	h.gfxdef
o.game:	h.global
o.game:	h.game
o.game:	h.maps
o.game:	h.global
o.game:	h.bitdraw
o.game:	h.bititem
o.game:	h.bitgroup
o.game:	h.gfxdef
o.game:	h.objlist
o.game:	h.sound
o.game:	h.snddef
o.game:	h.global
o.game:	h.player
o.game:	h.objlist
o.game:	h.objtypes
o.game:	h.baddie
o.game:	h.objlist
o.game:	h.suck
o.game:	h.objlist
o.game:	h.goodie
o.game:	h.objlist
o.game:	h.suck
o.game:	h.maps
o.game:	h.boss
o.game:	h.objlist
o.game:	h.suck
o.game:	h.misc
o.game:	h.objlist
o.game:	h.gasobj
o.game:	h.objlist
o.game:	h.power
o.game:	h.objlist
o.game:	h.weapon
o.game:	h.objlist
o.game:	h.target
o.game:	h.game
o.gasobj:	c++.gasobj
o.gasobj:	h.gasobj
o.gasobj:	h.objlist
o.gasobj:	h.global
o.gasobj:	h.bitgroup
o.gasobj:	h.gfxdef
o.gasobj:	h.global
o.gasobj:	h.player
o.gasobj:	h.objlist
o.gasobj:	h.maps
o.gasobj:	h.global
o.gasobj:	h.bitdraw
o.gasobj:	h.objtypes
o.gasobj:	h.suck
o.gasobj:	h.objlist
o.gasobj:	h.game
o.gasobj:	h.maps
o.gasobj:	h.bititem
o.gasobj:	h.bitgroup
o.gasobj:	h.objlist
o.gasobj:	h.sound
o.gasobj:	h.snddef
o.gasobj:	h.global
o.global:	c++.global
o.global:	h.global
o.goodie:	c++.goodie
o.goodie:	h.goodie
o.goodie:	h.objlist
o.goodie:	h.suck
o.goodie:	h.objlist
o.goodie:	h.maps
o.goodie:	h.global
o.goodie:	h.bitdraw
o.goodie:	h.global
o.goodie:	h.bitgroup
o.goodie:	h.gfxdef
o.goodie:	h.global
o.goodie:	h.player
o.goodie:	h.objlist
o.goodie:	h.objtypes
o.goodie:	h.misc
o.goodie:	h.objlist
o.goodie:	h.game
o.goodie:	h.maps
o.goodie:	h.bititem
o.goodie:	h.bitgroup
o.goodie:	h.objlist
o.goodie:	h.sound
o.goodie:	h.snddef
o.goodie:	h.global
o.maps:	c++.maps
o.maps:	h.global
o.maps:	h.maps
o.maps:	h.global
o.maps:	h.bitdraw
o.maps:	h.mapdef
o.maps:	h.target
o.maps:	h.game
o.maps:	h.maps
o.maps:	h.bititem
o.maps:	h.bitgroup
o.maps:	h.gfxdef
o.maps:	h.global
o.maps:	h.objlist
o.maps:	h.sound
o.maps:	h.snddef
o.maps:	h.global
o.misc:	c++.misc
o.misc:	h.misc
o.misc:	h.objlist
o.misc:	h.global
o.misc:	h.bitgroup
o.misc:	h.gfxdef
o.misc:	h.global
o.misc:	h.objtypes
o.misc:	h.game
o.misc:	h.maps
o.misc:	h.global
o.misc:	h.bitdraw
o.misc:	h.bititem
o.misc:	h.bitgroup
o.misc:	h.objlist
o.misc:	h.sound
o.misc:	h.snddef
o.misc:	h.global
o.misc:	h.player
o.misc:	h.objlist
o.misc:	h.goodie
o.misc:	h.objlist
o.misc:	h.suck
o.misc:	h.objlist
o.misc:	h.maps
o.misc:	h.gasobj
o.misc:	h.objlist
o.objlist:	c++.objlist
o.objlist:	h.objlist
o.objlist:	h.global
o.objlist:	h.player
o.objlist:	h.objlist
o.objlist:	h.game
o.objlist:	h.maps
o.objlist:	h.global
o.objlist:	h.bitdraw
o.objlist:	h.bititem
o.objlist:	h.bitgroup
o.objlist:	h.gfxdef
o.objlist:	h.global
o.objlist:	h.objlist
o.objlist:	h.sound
o.objlist:	h.snddef
o.objlist:	h.global
o.objlist:	h.bitgroup
o.objlist:	h.objtypes
o.player:	c++.player
o.player:	h.player
o.player:	h.objlist
o.player:	h.objlist
o.player:	h.gasobj
o.player:	h.objlist
o.player:	h.global
o.player:	h.bitgroup
o.player:	h.gfxdef
o.player:	h.global
o.player:	h.objtypes
o.player:	h.suck
o.player:	h.objlist
o.player:	h.game
o.player:	h.maps
o.player:	h.global
o.player:	h.bitdraw
o.player:	h.bititem
o.player:	h.bitgroup
o.player:	h.objlist
o.player:	h.sound
o.player:	h.snddef
o.player:	h.global
o.player:	h.goodie
o.player:	h.objlist
o.player:	h.suck
o.player:	h.maps
o.power:	c++.power
o.power:	h.misc
o.power:	h.objlist
o.power:	h.global
o.power:	h.bitgroup
o.power:	h.gfxdef
o.power:	h.global
o.power:	h.objtypes
o.power:	h.game
o.power:	h.maps
o.power:	h.global
o.power:	h.bitdraw
o.power:	h.bititem
o.power:	h.bitgroup
o.power:	h.objlist
o.power:	h.sound
o.power:	h.snddef
o.power:	h.global
o.power:	h.player
o.power:	h.objlist
o.power:	h.goodie
o.power:	h.objlist
o.power:	h.suck
o.power:	h.objlist
o.power:	h.maps
o.power:	h.power
o.power:	h.objlist
o.target:	c++.target
o.target:	h.target
o.target:	h.game
o.target:	h.maps
o.target:	h.global
o.target:	h.bitdraw
o.target:	h.bititem
o.target:	h.bitgroup
o.target:	h.gfxdef
o.target:	h.global
o.target:	h.objlist
o.target:	h.sound
o.target:	h.snddef
o.target:	h.global
o.target:	h.doc
o.target:	h.game
o.target:	h.target
o.suck:	c++.suck
o.suck:	h.suck
o.suck:	h.objlist
o.suck:	h.global
o.suck:	h.bitgroup
o.suck:	h.gfxdef
o.suck:	h.global
o.suck:	h.maps
o.suck:	h.global
o.suck:	h.bitdraw
o.suck:	h.objtypes
o.suck:	h.game
o.suck:	h.maps
o.suck:	h.bititem
o.suck:	h.bitgroup
o.suck:	h.objlist
o.suck:	h.sound
o.suck:	h.snddef
o.suck:	h.global
o.suck:	h.player
o.suck:	h.objlist
o.suck:	h.goodie
o.suck:	h.objlist
o.suck:	h.suck
o.suck:	h.maps
o.weapon:	c++.weapon
o.weapon:	h.weapon
o.weapon:	h.objlist
o.weapon:	h.global
o.weapon:	h.baddie
o.weapon:	h.objlist
o.weapon:	h.suck
o.weapon:	h.objlist
o.weapon:	h.bitgroup
o.weapon:	h.gfxdef
o.weapon:	h.global
o.weapon:	h.player
o.weapon:	h.objlist
o.weapon:	h.game
o.weapon:	h.maps
o.weapon:	h.global
o.weapon:	h.bitdraw
o.weapon:	h.bititem
o.weapon:	h.bitgroup
o.weapon:	h.objlist
o.weapon:	h.sound
o.weapon:	h.snddef
o.weapon:	h.global
o.weapon:	h.goodie
o.weapon:	h.objlist
o.weapon:	h.suck
o.weapon:	h.maps
o.weapon:	h.objtypes
o.sound:	c++.sound
o.sound:	h.sound
o.sound:	h.snddef
o.sound:	h.global
o.sound:	h.target
o.sound:	h.game
o.sound:	h.maps
o.sound:	h.global
o.sound:	h.bitdraw
o.sound:	h.bititem
o.sound:	h.bitgroup
o.sound:	h.gfxdef
o.sound:	h.global
o.sound:	h.objlist
o.sound:	h.sound
o.snddata:	c++.snddata
o.snddata:	h.global
o.snddata:	h.snddef
o.snddata:	h.global
o.gfxdata:	c++.gfxdata
o.gfxdata:	h.gfxdef
o.gfxdata:	h.global
o.doc:	c++.doc
o.doc:	h.swicalls
o.doc:	C:h.kernel
o.doc:	C:h.swis
o.doc:	h.doc
o.doc:	h.game
o.doc:	h.maps
o.doc:	h.global
o.doc:	h.bitdraw
o.doc:	h.bititem
o.doc:	h.bitgroup
o.doc:	h.gfxdef
o.doc:	h.global
o.doc:	h.objlist
o.doc:	h.sound
o.doc:	h.snddef
o.doc:	h.global
o.doc:	h.target
o.doc:	h.game
o.swicalls:	c++.swicalls
o.swicalls:	h.swicalls
o.swicalls:	C:h.kernel
o.swicalls:	C:h.swis
o.swicalls:	C:h.signal
o.swicalls:	C:h.kernel
