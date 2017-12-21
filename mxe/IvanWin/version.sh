#!/bin/bash

set -xue

CURRENT_GIT_TAG=$(git name-rev --name-only --tags HEAD)

if [ "$CURRENT_GIT_TAG" = "undefined" ]; then
  IVAN_FILE_VERSION="-unversioned-$(git rev-parse --short HEAD)-$(date +%Y%m%d%H%M%S)"
else
  IVAN_FILE_VERSION="${CURRENT_GIT_TAG#?}"
fi

set +xue
