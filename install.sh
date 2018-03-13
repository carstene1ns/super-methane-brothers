#!/bin/sh
# Author of this file Sigurd Stordal <sigurd_stordal@runbox.no>
# This is an install script for the methane brothers game.
# it will if necessary compile the program, it will create the hiscorefile and
# it will copy the game. It will not check the PATH variable to see it you have the installdir in the path

INSTALLDIR=/usr/local/games #Default installation directory is /usr/local/games
HISCOREDIR=/var/games       #Directory Hiscores are saved to, DO NOT CHANGE THIS


if [ ! -d $INSTALLDIR ]; then
    echo "$INSTALLDIR doesn't exist, creating it"
    mkdir $INSTALLDIR
fi
if [ ! -d $HISCOREDIR ]; then
    echo "$HISCOREDIR doesn't exist, creating it"
    mkdir $HISCOREDIR
fi

if test -w $HISCOREDIR/methanescores; then
    echo Hiscore file already exist, not creating a new one
else
    echo Creating empty Hiscorefile
    touch $HISCOREDIR/methanescores
fi

if test -x ./source/linux/methane ; then
    echo 'The game has already been compiled, proceeding to install'
    else
    cd ./source/linux
    make
    if [ $? -eq 0 ]; then
        echo "compiled methane without problems, proceding to install"
    else
        echo "unable to compile methane, try doing it manually, and report errors to game author"
        cd ..
        cd ..
        exit 127
    fi
    cd ..
    cd ..
fi

cp ./source/linux/methane $INSTALLDIR
echo "The game is now installed in $INSTALLDIR"
exit 0

