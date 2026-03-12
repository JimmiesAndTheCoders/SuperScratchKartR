@echo off
SET RAYLIB_PATH=C:\raylib\raylib
SET COMPILER_PATH=C:\raylib\w64devkit\bin
SET PATH=%COMPILER_PATH%;%PATH%

SET BUILD_DIR=build\windows

echo Building Super Scratch Kart R for Windows...

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

if exist "assets" (
    echo Syncing assets to %BUILD_DIR%...
    xcopy /E /I /Y "assets" "%BUILD_DIR%\assets"
)

setlocal enabledelayedexpansion
set "SOURCES="
set "INCLUDES=-Isrc"

for /r "src" %%f in (*.cpp) do (
    set "SOURCES=!SOURCES! "%%f""
)

for /d /r "src" %%d in (*) do (
    set "INCLUDES=!INCLUDES! -I"%%d""
)

echo Compiling...
g++ %SOURCES% -o %BUILD_DIR%\SuperScratchKartR.exe -O2 -Wall ^
    -I%RAYLIB_PATH%\src ^
    -L%RAYLIB_PATH%\src ^
    %INCLUDES% ^
    -lraylib -lopengl32 -lgdi32 -lwinmm

if %ERRORLEVEL% EQU 0 (
    echo Build Successful! Running game...
    pushd %BUILD_DIR%
    SuperScratchKartR.exe
    popd
) else (
    echo Build Failed.
    pause
)