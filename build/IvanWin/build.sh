#!/bin/bash

set -xue

MXE_DIR=/usr/lib/mxe

if [ "$IVAN_PLATFORM" = "IvanWin" ]; then
    MXE_TARGET=i686-w64-mingw32.static
fi

pkg-config --cflags sdl2
pkg-config --libs --static sdl2

${MXE_DIR}/usr/bin/${MXE_TARGET}-cmake . -Bbuild-dir -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON -DCMAKE_CXX_FLAGS="-Werror -Wno-format-security -Wno-undefined-var-template -Wno-unknown-warning-option $CXXFLAGS"
cmake --build build-dir --config Release

set +xue