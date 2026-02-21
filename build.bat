@echo off

SET RAYLIB_PATH=C:\raylib\raylib
SET COMPILER_PATH=C:\raylib\w64devkit\bin
SET PATH=%COMPILER_PATH%;%PATH%

:: Target Directory for Windows
SET BUILD_DIR=build\windows

echo Building Super Scratch Kart R for Windows...

:: Create specific platform directory if it doesn't exist
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

:: --- ASSET SYNC ---
if exist "assets" (
    echo Syncing assets to %BUILD_DIR%...
    xcopy /E /I /Y "assets" "%BUILD_DIR%\assets"
)

:: Compile and output to the platform directory
g++ src/*.cpp -o %BUILD_DIR%\SuperScratchKartR.exe -O2 -Wall -I%RAYLIB_PATH%\src -L%RAYLIB_PATH%\src -Iinclude -lraylib -lopengl32 -lgdi32 -lwinmm

if %ERRORLEVEL% EQU 0 (
    echo Build Successful! Running game...
    cd %BUILD_DIR%
    SuperScratchKartR.exe
    cd ..\..
) else (
    echo Build Failed.
    pause
)