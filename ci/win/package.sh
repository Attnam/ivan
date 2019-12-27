#!/bin/bash

set -xue

if [ -z "${IVAN_BUILD_DIR}" ]; then
  echo 'Environment variable IVAN_BUILD_DIR is empty!'
  echo "IVAN_BUILD_DIR=${IVAN_BUILD_DIR}"
  exit 1
fi

if [ -n "${IVAN_PLATFORM}" ]; then
  IVAN_PLATFORM=win
fi

BUILD_DIR="${IVAN_BUILD_DIR}/${IVAN_PLATFORM}"
FILENAME="IVAN-v${IVAN_FILE_VERSION:-000}-${IVAN_PLATFORM}"
GAME_DIR="${BUILD_DIR}/${FILENAME}"
ZIP_FILE="${BUILD_DIR}/${FILENAME}.zip"

mv "${BUILD_DIR}/ivan" "${GAME_DIR}"

# for a good-looking zipball
cd "${GAME_DIR}/.."
GAME_DIR=$(basename "${GAME_DIR}")

zip -9 -r "${ZIP_FILE}" "${GAME_DIR}"
