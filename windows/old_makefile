
CC="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\bin\Hostx64\x64\cl.exe"
LD="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\bin\Hostx64\x64\link.exe"
AR="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\bin\Hostx64\x64\lib.exe"

INCLUDE_DIRS = include graphics/include

LIB_DIRS = lib

# todo(brandon): determine no logo option


LIB_LIST = user32.lib GDI32.lib Kernel32.lib $(MY_LIB_LIST)


CPP_SOURCES = src/main.cpp src/console_redirect.cpp src/console_another.cpp 


OBJ_FILES = $(subst .cpp,.o,$(subst src,bin,$(CPP_SOURCES)))


all: bin/hello.exe

clean:
	rm -rf bin


lib/MyGraphics.lib: bin/graphics/bin/video.o
	mkdir -p $(dir $@)
	$(AR) /OUT:$@ $^ 

bin/graphics/bin/video.o: graphics/src/video.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) /std:c++17 -c -Fo$@ $<

bin/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) /std:c++17 -c -Fo$@ $<


bin/hello.exe: $(OBJ_FILES) lib/MyGraphics.lib
	$(LD) /OUT:$@ $(foreach lib_d,$(LIB_DIRS),/LIBPATH:$(lib_d)) $(LIB_LIST) $^ 
