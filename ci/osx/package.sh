#!/bin/bash

# Required: ${IVAN_BUILD_DIR} ${SDL2DIR} and pkg-config
# Optional: ${IVAN_PLATFORM}

set -e

if [[ -z "${IVAN_BUILD_DIR}" || -z "${SDL2DIR}" ]]; then
  echo 'Environment variables IVAN_BUILD_DIR or SDL2DIR is empty!'
  echo "IVAN_BUILD_DIR=${IVAN_BUILD_DIR}"
  echo "SDL2DIR=${SDL2DIR}"
  exit 1
fi

if [[ -n "${IVAN_PLATFORM}" ]]; then
  set -x
else
  IVAN_PLATFORM=osx
fi

BUILD_DIR="${IVAN_BUILD_DIR}/${IVAN_PLATFORM}"
FILENAME="IVAN-v${IVAN_FILE_VERSION:-000}-${IVAN_PLATFORM}"
GAME_DIR="${BUILD_DIR}/${FILENAME}"
ZIP_FILE="${BUILD_DIR}/${FILENAME}.zip"

# directory of this script
SCRIPT_DIR=$(dirname -- "$0")

mkdir -p "${GAME_DIR}"
cp -R "${SCRIPT_DIR}/IVAN.app" "${GAME_DIR}"

DATA_DIR="${GAME_DIR}/IVAN.app/Contents"

mkdir -p "${DATA_DIR}"/{MacOS,Resources,Frameworks}
cp "${BUILD_DIR}"/bin/ivan "${DATA_DIR}/MacOS"
cp -R "${BUILD_DIR}"/docs/* "${GAME_DIR}"
cp -R "${BUILD_DIR}"/ivan "${DATA_DIR}/Resources"
cp -R "${SDL2DIR}"/*.framework "${DATA_DIR}/Frameworks"

BIN="${DATA_DIR}/MacOS/ivan"
install_name_tool -add_rpath @loader_path/../Frameworks "${BIN}"

find_lib() {
  otool -D "$(pkg-config --variable libdir -- "$1")/${1}.dylib" | tail -n1
}

libpcre=$(find_lib libpcre)
cp "${libpcre}" "${DATA_DIR}/Frameworks/libpcre.dylib"
install_name_tool -change "${libpcre}" @loader_path/../Frameworks/libpcre.dylib "${BIN}"

libpng=$(find_lib libpng)
cp "${libpng}" "${DATA_DIR}/Frameworks/libpng.dylib"
install_name_tool -change "${libpng}" @loader_path/../Frameworks/libpng.dylib "${BIN}"

# for a good-looking zipball
cd "${GAME_DIR}/.."
GAME_DIR=$(basename "${GAME_DIR}")

if [[ -n "${IVAN_PLATFORM}" ]]; then
  zip -9 -r --symlinks "${ZIP_FILE}" "${GAME_DIR}" | grep -v Headers
else
  zip -9 -r --symlinks "${ZIP_FILE}" "${GAME_DIR}"
fi
