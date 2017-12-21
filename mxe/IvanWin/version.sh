#!/bin/bash

set -xue

CURRENT_GIT_TAG=$(git describe --exact-match --tags HEAD)

if [ -z $CURRENT_GIT_TAG ]; then
  IVAN_FILE_VERSION="-unversioned-$(git rev-parse --short HEAD)-$(date +%Y%m%d%H%M%S)"
else
  IVAN_FILE_VERSION="${CURRENT_GIT_TAG#?}"
fi

set +xue
