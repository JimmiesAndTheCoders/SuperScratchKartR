#!/bin/bash

# Detect OS for directory naming
if [[ "$OSTYPE" == "darwin"* ]]; then
    OS_NAME="macos"
else
    OS_NAME="linux"
fi

BUILD_DIR="build/$OS_NAME"

echo "Building Super Scratch Kart R for $OS_NAME..."

# Create platform directory
mkdir -p "$BUILD_DIR"

# --- ASSET SYNC ---
if [ -d "assets" ]; then
    echo "Syncing assets to $BUILD_DIR..."
    cp -r assets "$BUILD_DIR/"
fi

# Compile based on OS
if [ "$OS_NAME" == "macos" ]; then
    # macOS build command
    g++ src/*.cpp -Iinclude -o "$BUILD_DIR/SuperScratchKartR" -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
else
    # Linux build command
    g++ src/*.cpp -Iinclude -o "$BUILD_DIR/SuperScratchKartR" -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
fi

if [ $? -eq 0 ]; then
    echo "Build Successful! Running game..."
    cd "$BUILD_DIR"
    ./SuperScratchKartR
    cd ../..
else
    echo "Build Failed."
fi