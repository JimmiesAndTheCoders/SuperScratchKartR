@echo off

SET RAYLIB_PATH=C:\raylib\raylib
SET COMPILER_PATH=C:\raylib\w64devkit\bin

SET PATH=%COMPILER_PATH%;%PATH%

echo Building Super Scratch Kart R...

gcc src/*.cpp -o SuperScratchKartR.exe -O2 -Wall -I%RAYLIB_PATH%\src -L%RAYLIB_PATH%\src -lraylib -lopengl32 -lgdi32 -lwinmm

if %ERRORLEVEL% EQU 0 (
    echo Build Successful! Running game...
    SuperScratchKartR.exe
) else (
    echo Build Failed.
    pause
)