#!/bin/bash

set -e

if [[ -n "${IVAN_PLATFORM}" ]]; then
  set -x
fi

CACHE_DIR="${SDL2DIR}"

# directory of this script
SCRIPT_DIR=$(dirname -- "$0")

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
  local dirs=(
    'SDL2.framework'
    'SDL2_mixer.framework'
  )
  local urls=(
    'https://libsdl.org/release/SDL2-2.0.20.dmg'
    'https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.4.dmg'
  )

  mkdir -p "${CACHE_DIR}"

  local i
  for i in "${!urls[@]}"; do
    local dir="${dirs[$i]}" url="${urls[$i]}"

    if [[ -d "${CACHE_DIR}/${dir}" ]]; then
      continue
    fi

    local tmpdir="$(mktemp -d)"
    trap 'hdiutil detach sdl >/dev/null 2>&1 || true; rm -Rf "${tmpdir}"' RETURN
    pushd "${tmpdir}"

    curl -L --fail --retry 5 -o sdl.dmg "${url}"
    hdiutil attach -readonly -mount required -mountpoint sdl sdl.dmg
    cp -R sdl/"${dir}" "${CACHE_DIR}"
    hdiutil detach sdl
    rm sdl.dmg

    popd
    rm -Rf "${tmpdir}"
  done

  echo "Cached Frameworks:"
  ls -hl "${CACHE_DIR}"
}

if [[ -n "${IVAN_PLATFORM}" || "${BUILD_MAC_APP}" = ON ]]; then
  ## pcre and libpng are quiet stable, the CI servers can do better here
  #if [[ -n "${TRAVIS_TAG}" || "${TODO_BREW_UPDATE}" = ON ]]; then
  #  brew update  # for deployment
  #fi
  brew_install pkg-config cmake
  if [[ -n "${MACOSX_DEPLOYMENT_TARGET}" ]]; then
    # dyld: lazy symbol binding failed: Symbol not found: ____chkstk_darwin
    for formula in "${SCRIPT_DIR}"/{pcre,libpng}.rb; do
      # brew will sanitize environment variables, therefore...
      sed -i.orig "s/X_MACOSX_DEPLOYMENT_TARGET/${MACOSX_DEPLOYMENT_TARGET}/g" "${formula}"
      brew reinstall --force --build-from-source --formula "${formula}"
      mv "${formula}"{.orig,}
    done
  else
    brew_install pcre libpng
  fi
  install_sdl2
else
  brew_install pkg-config cmake pcre libpng sdl2 sdl2_mixer
fi
