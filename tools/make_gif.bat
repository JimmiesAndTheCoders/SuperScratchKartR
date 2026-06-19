@echo off
REM Usage: make_gif.bat recording.mp4 out.gif [start] [end] [width]
if "%1"=="" (
  echo Usage: %~nx0 input.mp4 output.gif [start] [end] [width]
  exit /b 1
)
set INPUT=%~1
set OUTPUT=%~2
set START=%~3
set END=%~4
set WIDTH=%~5
if "%WIDTH%"=="" set WIDTH=640

REM Temporary palette file
set PAL=%TEMP%\palette.png

if not "%START%"=="" (
  ffmpeg -ss %START% -to %END% -i "%INPUT%" -vf "fps=15,scale=%WIDTH%:-1:flags=lanczos,palettegen" -y "%PAL%"
  ffmpeg -ss %START% -to %END% -i "%INPUT%" -i "%PAL%" -lavfi "fps=15,scale=%WIDTH%:-1:flags=lanczos [x]; [x][1:v] paletteuse" -y "%OUTPUT%"
) else (
  ffmpeg -i "%INPUT%" -vf "fps=15,scale=%WIDTH%:-1:flags=lanczos,palettegen" -y "%PAL%"
  ffmpeg -i "%INPUT%" -i "%PAL%" -lavfi "fps=15,scale=%WIDTH%:-1:flags=lanczos [x]; [x][1:v] paletteuse" -y "%OUTPUT%"
)

if exist "%PAL%" del "%PAL%"
echo Created %OUTPUT%
exit /b 0
