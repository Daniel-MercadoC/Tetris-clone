@echo off

subst w: "C:\Users\Kakkon\GameDev\Raylib\Tetris"

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

set path=w:\misc\;%path%

w:
