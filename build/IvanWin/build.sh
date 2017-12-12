#!/bin/bash

set -xue

MXE_DIR=/usr/lib/mxe

if [ "$BATTLESHIP_PLATFORM" = "IvanWin" ]; then
    MXE_TARGET=i686-w64-mingw32.static
fi

${MXE_DIR}/usr/bin/${MXE_TARGET}-cmake . -Bbuild-dir -DCMAKE_CXX_FLAGS="-Werror -Wno-format-security -Wno-undefined-var-template -Wno-unknown-warning-option $CXXFLAGS"
cmake --build build-dir --config Release

set +xue
