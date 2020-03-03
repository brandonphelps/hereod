
CC="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\bin\Hostx64\x64\cl.exe"
LD="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\bin\Hostx64\x64\link.exe"


INCLUDE_DIRS = include

# todo(brandon): determine no logo option
C_FLAGS = -nologo -DWIN32 -EHsc $(foreach tmp_dir,$(INCLUDE_DIRS),-I$(tmp_dir))

LIB_LIST = user32.lib GDI32.lib Kernel32.lib




CPP_SOURCES = src/main.cpp src/console_redirect.cpp src/console_another.cpp

OBJ_FILES = $(subst .cpp,.o,$(subst src,bin,$(CPP_SOURCES)))

all: bin/hello.exe

clean:
	rm -rf bin

bin/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) /std:c++17 -c -Fo$@ $<


bin/hello.exe: $(OBJ_FILES)
	$(LD) $^ /OUT:$@ $(LIB_LIST)
