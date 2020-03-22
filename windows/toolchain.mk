


CC="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\bin\Hostx64\x64\cl.exe"
LD="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\bin\Hostx64\x64\link.exe"
AR="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\bin\Hostx64\x64\lib.exe"

C_FLAGS = -nologo -DWIN32 -EHsc /std:c++17 

LIBS = user32.lib GDI32.lib Kernel32.lib Winmm.lib

C_OUTPUT_SPECIFIER = -Fo
C_BIN_SPECIFIER = -c

INCLUDE_DIRS += -Iwindows/include

TARGETS = cool.exe


TARGET_OBJS = 
