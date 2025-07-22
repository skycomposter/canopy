#!/bin/bash

# Platform identifier.
MACOS="macos"

# Directory paths.
SCRIPT_DIR="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"
ROOT_DIR="$SCRIPT_DIR/.."
ENGINE_DIR="$ROOT_DIR/canopy-core"
MACOS_DIR="$ROOT_DIR/canopy-macos/Canopy/libraries/libcanopy"

print_usage() {
    echo "usage:"
    echo "    update_engine.sh $MACOS"
}

update_macos() {
    # Build the engine.
    make -C "$ENGINE_DIR"
    # Copy the headers and compiled library to the macOS project.
    cp "$ENGINE_DIR/src/canopy_engine.h" "$MACOS_DIR/include/"
    cp "$ENGINE_DIR/src/input.h" "$MACOS_DIR/include/"
    cp "$ENGINE_DIR/src/input_codes.h" "$MACOS_DIR/include/"
    cp "$ENGINE_DIR/src/input_event.h" "$MACOS_DIR/include/"
    cp "$ENGINE_DIR/src/util/primitives.h" "$MACOS_DIR/include/util/"
    cp "$ENGINE_DIR/out/libcanopy.a" "$MACOS_DIR/lib/"
}

if [ "$#" -lt "1" ] || [ "$#" -gt "1" ] || [ "$1" != "$MACOS" ]; then
    print_usage
    exit -1 
fi

update_macos
