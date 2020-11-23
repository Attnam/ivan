#!/bin/bash

set -e

if [[ -n "${IVAN_PLATFORM}" ]]; then
  set -x
fi

CACHE_DIR="${SDL2DIR}"

brew_install() {
  for dep in "$@"; do
    if brew list "${dep}" &>/dev/null; then
      if [[ -n "${IVAN_PLATFORM}" || "${BUILD_MAC_APP}" = ON ]]; then
        : brew upgrade "${dep}" || true  # error for "already installed" is so annoying
      fi
    else
      brew install "${dep}"
    fi
  done
}

install_sdl2() {
  local urls=(
    'https://libsdl.org/release/SDL2-2.0.9.dmg'
    'https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.4.dmg'
  )

  mkdir -p "${CACHE_DIR}"
  if [[ "${CACHE_DIR}" = "$(find "${CACHE_DIR}" -maxdepth 0 -type d -empty)" ]]
  then
    local tmpdir=$(mktemp -d)
    pushd "${tmpdir}"

    local url
    for url in "${urls[@]}"; do
      curl -L --fail --retry 5 -o sdl.dmg "${url}"
      hdiutil attach -readonly -mount required -mountpoint sdl sdl.dmg
      cp -R sdl/* "${CACHE_DIR}"
      hdiutil detach sdl
      rm sdl.dmg
    done

    popd
    rm -Rf "${tmpdir}"
  fi

  echo "Cached Frameworks:"
  ls -hl "${CACHE_DIR}"
}

if [[ -n "${IVAN_PLATFORM}" || "${BUILD_MAC_APP}" = ON ]]; then
  if [[ -n "${TRAVIS_TAG}" ]]; then
    brew update  # for deployment
  fi
  brew_install pkg-config cmake pcre libpng
  install_sdl2
else
  brew_install pkg-config cmake pcre libpng sdl2 sdl2_mixer
fi
