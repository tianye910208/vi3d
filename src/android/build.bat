rd /q /s obj

rd /q /s assets
mkdir assets
mkdir assets\lua
mkdir assets\res

xcopy /S ..\..\lua assets\lua
xcopy /S ..\..\res assets\res

call ndk-build
pause

call ant debug
pause