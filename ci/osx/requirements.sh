#!/bin/bash

set -e

if [[ -n "${IVAN_PLATFORM}" ]]; then
  set -x
fi

CACHE_DIR="${SDL2DIR}"

brew_install() {
  for dep in "$@"; do
    if brew list "${dep}" &>/dev/null; then
      brew upgrade "${dep}" || true  # error for "already installed" is so annoying
    else
      brew install "${dep}"
    fi
  done
}

install_sdl2() {
  local urls=(
    'https://libsdl.org/release/SDL2-2.0.8.dmg'
    'https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.2.dmg'
  )

  mkdir -p "${CACHE_DIR}"
  if [[ "${CACHE_DIR}" = "$(find "${CACHE_DIR}" -maxdepth 0 -type d -empty)" ]]
  then
    local tmpdir=$(mktemp -d)
    pushd "${tmpdir}"

    for url in "${urls[@]}"; do
      curl -L --fail --retry 5 -o sdl.dmg "${url}"
      hdiutil mount -readonly -mountpoint sdl sdl.dmg
      cp -R sdl/* "${CACHE_DIR}/"
      hdiutil unmount sdl
      rm sdl.dmg
    done

    popd
    rm -Rf "${tmpdir}"
  fi

  echo "Cached Frameworks:"
  ls -hl "${CACHE_DIR}"
}

brew update

if [[ -n "${IVAN_PLATFORM}" || "${BUILD_MAC_APP}" = ON ]]; then
  brew_install pkg-config cmake pcre libpng
  install_sdl2
else
  brew_install pkg-config cmake pcre libpng sdl2 sdl2_mixer
fi
