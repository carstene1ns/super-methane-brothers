.PHONY = all wii gc wii-clean gc-clean wii-run gc-run

all: wii gc

run: wii-run

clean: wii-clean gc-clean

wii:
	@echo Compiling for Wii..
	@$(MAKE) --no-print-directory -f Makefile.gx GXPLATFORM=wii

wii-clean:
	@$(MAKE) --no-print-directory -f Makefile.gx clean GXPLATFORM=wii

wii-run: wii
	@$(MAKE) --no-print-directory -f Makefile.gx run GXPLATFORM=wii

gc:
	@echo Compiling for GameCube..
	@$(MAKE) --no-print-directory -f Makefile.gx GXPLATFORM=gc

gc-clean:
	@$(MAKE) --no-print-directory -f Makefile.gx clean GXPLATFORM=gc

gc-run: gc
	@$(MAKE) --no-print-directory -f Makefile.gx run GXPLATFORM=gc
