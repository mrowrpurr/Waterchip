@echo off

:: Make a Waterchip.dat
cd data\scripts\
dat2 a -t scripts Waterchip.dat gl___waterchip_test_runner.int
cd ..\..
move data\scripts\Waterchip.dat .

:: Make mods\ and scripts\HEADERS\ folders for our files to go into (in the .zip)
mkdir release\mods
mkdir release\scripts\HEADERS

:: Copy/move our files in there
move Waterchip.dat release\mods\
copy scripts\headers\Waterchip.h release\scripts\HEADERS\

:: Make the .zip
cd release
7z a Waterchip.zip .

:: Wait a moment before deleting otherwise 7z might still be archiving...
timeout /t 1

:: Move the .zip to the root and delete the release\ folder
cd .. 
move release\Waterchip.zip .
rmdir /S /Q release
