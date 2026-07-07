#!/usr/bin/env bash

source tools/common.sh

step "Building projects"

for project in $(discover_projects); do

    echo "Building $(basename "$project")"

    (
        cd "$project"
        idf.py build
    )

done

success "Build passed"
