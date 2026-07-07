#!/usr/bin/env bash

source tools/common.sh

step "Running clang-format"

find project \
    \( \
        -path "*/build" -o \
        -path "*/managed_components" \
    \) -prune -o \
    -type f \
    \( \
        -name "*.c" -o \
        -name "*.cpp" -o \
        -name "*.h" \
    \) \
    -print0 |
xargs -0 clang-format --dry-run --Werror

success "clang-format passed"
