#!/bin/bash

if [[ "$OSTYPE" == "darwin"* ]]; then
    OS_NAME="macos"
else
    OS_NAME="linux"
fi

BUILD_DIR="build/$OS_NAME"
echo "Building Super Scratch Kart R for $OS_NAME..."

mkdir -p "$BUILD_DIR"

if [ -d "assets" ]; then
    echo "Syncing assets to $BUILD_DIR..."
    cp -r assets "$BUILD_DIR/"
fi

SOURCES=$(find src -name "*.cpp")

FLAGS="-O2 -Wall -Isrc"

if [ "$OS_NAME" == "macos" ]; then
    g++ $SOURCES $FLAGS -o "$BUILD_DIR/SuperScratchKartR" -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
else
    g++ $SOURCES $FLAGS -o "$BUILD_DIR/SuperScratchKartR" -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
fi

if [ $? -eq 0 ]; then
    echo "Build Successful! Running game..."
    cd "$BUILD_DIR" && ./SuperScratchKartR
else
    echo "Build Failed."
fi