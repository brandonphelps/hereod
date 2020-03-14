
include mac_osx/toolchain.mk

INCLUDE_DIRS = -Imaco_osx/include -Iinclude

C_FLAGS += $(INCLUDE_DIRS)

cool: bin/main.o bin/video.o
	$(LD) $(LD_FLAGS) -o cool $^ -L/usr/local/lib -lc++ -lSystem /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/11.0.0/lib/darwin/libclang_rt.osx.a


bin/main.o: mac_osx/src/main.mm
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $< -o $@

bin/video.o: src/video.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $< -o $@

clean:
	rm -rf bin cool


print_predefines:
	$(CC) -dM -E -x c++ - < /dev/null
