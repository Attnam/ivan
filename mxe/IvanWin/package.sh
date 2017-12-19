#!/bin/bash

set -xue

if [ "$IVAN_PLATFORM" = "IvanWin" ]; then
  zip -9 -y -r -q IvanWin.zip ${TRAVIS_BUILD_DIR}/build/IvanWin
fi

set +xue
