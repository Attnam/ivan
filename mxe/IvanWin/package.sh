#!/bin/bash

set -xue

if [ "$IVAN_PLATFORM" = "IvanWin" ]; then
  cd ${TRAVIS_BUILD_DIR}/build/IvanWin/
  sudo mv ivan ${IVAN_PLATFORM}
  sudo zip -9 -r -j ${IVAN_PLATFORM}.zip *
fi

set +xue
