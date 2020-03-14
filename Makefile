.SUFFIXES:
.PREFIXES:

CC = 
LD = 

ifeq ($(OS),Windows_NT)

include windows/toolchain.mk

else

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)

include mac_osx/toolchain.mk

endif

endif

# include mac_osx/toolchain.mk

$(info $(CC))

INCLUDE_DIRS = -Imaco_osx/include -Iinclude

LIB_DIRS = -L/usr/local/lib

LIBS = -lc++ -lSystem /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/11.0.0/lib/darwin/libclang_rt.osx.a

C_FLAGS += $(INCLUDE_DIRS)

cool: bin/main.o bin/video.o
	$(LD) $(LD_FLAGS) $(LIBS) -o $@ $^ $(LIB_DIRS)

bin/main.o: mac_osx/src/main.mm
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $< $(C_OUTPUT_SPECIFIER) $@

bin/video.o: src/video.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $< -o $@

clean:
	rm -rf bin cool

print_predefines:
	$(CC) -dM -E -x c++ - < /dev/null
