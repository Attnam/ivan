#!/bin/bash

set -xue

if [ "$IVAN_PLATFORM" = "IvanWin" ]; then
  cd ${TRAVIS_BUILD_DIR}/build/IvanWin/
  mv ivan ${IVAN_PLATFORM}${IVAN_FILE_VERSION}
  zip -9 -r ${IVAN_PLATFORM}${IVAN_FILE_VERSION}.zip *
fi

set +xue
