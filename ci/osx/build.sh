#!/bin/bash

# Optional: ${IVAN_PLATFORM} ${IVAN_BUILD_DIR} ${SDL2DIR}

set -e

if [[ -n "${IVAN_PLATFORM}" ]]; then
  set -x
  BUILD_MAC_APP=ON
else
  IVAN_PLATFORM=osx
fi

IVAN_BUILD_DIR=${IVAN_BUILD_DIR:-"${PWD}"}

cmake .. \
    -DBUILD_MAC_APP="${BUILD_MAC_APP}" \
    -DCMAKE_CXX_FLAGS="-Werror -Wno-undefined-var-template -Wno-unknown-warning-option ${CXXFLAGS}" \
    -DCMAKE_INSTALL_PREFIX="${IVAN_BUILD_DIR}/${IVAN_PLATFORM}"
