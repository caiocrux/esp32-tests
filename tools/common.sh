#!/usr/bin/env bash

set -euo pipefail

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

step() {
    echo
    echo -e "${BLUE}==> $1${NC}"
}

success() {
    echo -e "${GREEN}✔ $1${NC}"
}

error() {
    echo -e "${RED}✘ $1${NC}"
    exit 1
}

require() {
    command -v "$1" >/dev/null 2>&1 || error "$1 is not installed"
}

PROJECT_ROOT="project"

discover_projects() {
    find "$PROJECT_ROOT" \
        -mindepth 1 \
        -maxdepth 1 \
        -type d \
        -exec test -f "{}/CMakeLists.txt" \; \
        -print
}
