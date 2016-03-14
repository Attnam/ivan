AC_INIT([Iter Vehemens ad Necem], [0.50.4],
        [http://github.com/Attnam/ivan/issues],
        [ivan], [http://github.com/Attnam/ivan])
AC_CONFIG_SRCDIR([Main/Source/main.cpp])

AM_PROG_AS 
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE
AC_PROG_CC

AC_PROG_CXX


dnl Check for SDL2
SDL2_VERSION=2.0.0
AM_PATH_SDL2($SDL2_VERSION,
             :,
             AC_MSG_WARN([*** SDL2 version $SDL2_VERSION not found!]))

if test "$SDL2_CONFIG" = "no"; then
    dnl Check for SDL
    SDL_VERSION=1.2.0
    AM_PATH_SDL($SDL_VERSION,
                :,
                AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!]))
fi

localstatedir=/var/lib
CFLAGS="$CFLAGS $SDL_CFLAGS "
LIBS="$LIBS $SDL_LIBS $SDL2_LIBS"
CPPFLAGS="$SDL_CFLAGS $SDL2_CFLAGS -DLINUX -DLOCAL_STATE_DIR=\\\"$localstatedir/ivan\\\" -DDATADIR=\\\"$datadir\\\" -DUSE_SDL -DGCC -std=c++11"

AC_PROG_INSTALL

AC_PROG_RANLIB

AC_CONFIG_FILES(Makefile FeLib/Makefile FeLib/Include/Makefile
                FeLib/Source/Makefile Script/Makefile Graphics/Makefile
                Main/Makefile Main/Include/Makefile Main/Resource/Makefile
                Main/Source/Makefile)
AC_OUTPUT
