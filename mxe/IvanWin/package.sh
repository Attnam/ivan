#!/bin/bash

set -xue

if [ "$IVAN_PLATFORM" = "IvanWin" ]; then
  cd ${TRAVIS_BUILD_DIR}/build/IvanWin/
  mv ivan ${IVAN_PLATFORM}
  zip -9 -r ${IVAN_PLATFORM}.zip *
fi

set +xue
