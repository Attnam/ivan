#!/bin/bash

set -xue

if [ "$IVAN_PLATFORM" = "IvanWin" ]; then
  zip -9 -r ${IVAN_PLATFORM}.zip ${TRAVIS_BUILD_DIR}/build/IvanWin/*
fi

set +xue
