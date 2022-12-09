# Super Methane Brothers (homebrew edition)

Trap baddies in a gas cloud. Suck into your gas gun. Throw against a wall to
destroy them. Survive the 100 floors of the Tower of Time.

Puff and Blow each have a Methane Gas Gun which fires a cloud of immobilising
gas. If this comes into contact with a bad guy he will be absorbed into the gas
and then float around the screen for a limited time. Bad guys are harmless in
this state. Puff and Blow must suck the floating gas clouds into their guns and
blast them out against a vertical surface. Bad guys then turn into bonuse
which can be collected. Be warned! - the gas cloud dissolves with time after
which baddies become active again and extremely annoyed.

## General Instructions

* Use the `Stick`/`Hat` to move around the screen.
* Tap the `Fire` button to fire gas from the gun.
* Hold the `Fire` button to suck a trapped baddie into the gun.
* Release the `Fire` button to throw the trapped baddie from the gun.
* Throw baddies at the wall to destroy them.


| Button | GameCube | Wii | Switch |
|--------|----------|-----|--------|
| Fire   | A        |     |        |
| Start Game, single player | A (controller 1) |
| Start Game, 2 players | A (controller 2) |
| Pause Game | START |
| Switch player graphics | Z |

### Wii port:

* Press the A button on controller 2 for 2 player mode.
* Press START button to pause the game.
* Press Z button to switch player graphics.

### Switch port:

* Press the A button on controller 1 to start the game in single player
* Press MINUS button to pause the game.
* Press START button to exit the game.
* Press X button to switch player graphics.

See [docs/info.html](docs/info.html) for further instructions on how to play
the game.

## Building

```bash
make -f Makefile.switch       # for the Switch version
make -f Makefile.gx           # for the Wii version
make -f Makefile.gx DEVICE=gc # for the GameCube version
make -f Makefile.oldgc        # for the outdated GameCube port.
```

## Port history

| Platform | Date    | Notes                                |
|----------|--------:|--------------------------------------|
| GameCube | 11/2012 | Using DevkitPPC, libogc, cubeSDL,
                       libaesnd and libmodplay              |
| Wii/GC   |    2012 |                                      |
| Switch   | 04/2018 | Using DevkitA64 and libnx            |
| 3DS      | 07/2019 | Using DevkitARM and libctru          |

## License

* See [README](README) for original readme file
* See [COPYING](COPYING) for GPL licence
