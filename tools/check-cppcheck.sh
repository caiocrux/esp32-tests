#!/usr/bin/env bash

source tools/common.sh

step "Running cppcheck"

cppcheck \
    --enable=warning,style,performance,portability \
    --inline-suppr \
    --std=c11 \
    --error-exitcode=1 \
    project

success "cppcheck passed"
