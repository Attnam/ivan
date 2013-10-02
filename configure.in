AC_INIT(Main/Source/main.cpp)

AM_PROG_AS 
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE(ivan,0.50)
AC_PROG_CC

AC_PROG_CXX

dnl Check for SDL
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION,
            :,
            AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)

CFLAGS="$CFLAGS $SDL_CFLAGS "
LIBS="$LIBS $SDL_LIBS"
CPPFLAGS="$SDL_CFLAGS -DLINUX -DLOCAL_STATE_DIR=\\\"$sharedstatedir/ivan\\\" -DDATADIR=\\\"$datadir\\\" -DUSE_SDL -DGCC"

AC_PROG_INSTALL

AC_PROG_RANLIB

AC_OUTPUT(Makefile FeLib/Makefile FeLib/Include/Makefile FeLib/Source/Makefile Script/Makefile Graphics/Makefile Main/Makefile Main/Include/Makefile Main/Resource/Makefile Main/Source/Makefile)
