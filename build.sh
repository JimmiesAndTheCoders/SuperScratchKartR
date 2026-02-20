#!/bin/bash

# Check if we are on macOS or Linux
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Building for macOS..."
    # macOS build command (assumes raylib is installed via Homebrew)
    g++ src/*.cpp -Iinclude -o SuperScratchKartR -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
else
    echo "Building for Linux..."
    # Linux build command
    g++ src/*.cpp -Iinclude -o SuperScratchKartR -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
fi

if [ $? -eq 0 ]; then
    echo "Build Successful! Running game..."
    ./SuperScratchKartR
else
    echo "Build Failed."
fi