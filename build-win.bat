@echo off
REM Example build script for Microsoft Windows

if not exist build mkdir build
cd build
cmake -D"OpenCV_DIR":STRING="C:\OpenCV" -D"Microsoft.GSL_DIR":STRING="C:\GSL\share\cmake\Microsoft.GSL" ..
