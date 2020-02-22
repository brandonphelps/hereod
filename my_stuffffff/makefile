
CC="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\bin\Hostx64\x64\cl.exe"
LD="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\bin\Hostx64\x64\link.exe"

# todo(brandon): determine no logo option
C_FLAGS = -NOLOGO

all: bin/hello.exe


clean:
	rm -rf bin

bin/main.o: src/main.cpp
	mkdir -p $(dir $@)
	$(CC) /std:c++17 -c -Fo$@ $<


bin/hello.exe: bin/main.o
	$(LD) $^ /OUT:$@
