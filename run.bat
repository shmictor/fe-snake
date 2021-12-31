set name=snake
set flags=/nologo /W3 /Fe:%name%
set files=lib\*.c back\*.c
set libs=opengl32.lib glfw3dll.lib

call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cl %flags% %files% %libs%
%name%.exe
del *.obj