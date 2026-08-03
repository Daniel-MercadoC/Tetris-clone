@echo off

pushd ..\build

cl -o ..\main ..\src\main.c ..\src\board.c ..\src\game.c ..\src\input.c ..\src\tetromino.c ..\src\render.c -I ..\include\ -Zi ..\lib\raylib.lib gdi32.lib User32.lib shell32.lib winmm.lib opengl32.lib kernel32.lib msvcrt.lib

popd
