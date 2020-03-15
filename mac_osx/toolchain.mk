LD=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ld
LD_COMMON = -arch x86_64 -syslibroot /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk
LD_FLAGS= -framework AppKit -framework IOKit -demangle -lto_library /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/libLTO.dylib -no_deduplicate $(LD_COMMON) -macosx_version_min 10.14.0 
LD_DLL_FLAGS = -demangle -lto_library /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/libLTO.dylib -no_deduplicate -dynamic -dylib -arch x86_64 -macosx_version_min 10.14.0 -syslibroot /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk -L/usr/local/lib -lSystem /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/11.0.0/lib/darwin/libclang_rt.osx.a

CC=clang
C_FLAGS = -cc1 -triple x86_64-apple-macosx10.14.0 -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -mrelax-all -disable-free -disable-llvm-verifier -discard-value-names -main-file-name main.mm -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -fno-strict-return -masm-verbose -munwind-tables -target-sdk-version=10.14 -target-cpu penryn -dwarf-column-info -debugger-tuning=lldb -ggnu-pubnames -target-linker-version 530 -coverage-notes-file /Users/Elemental/Downloads/SDL2-2.0.12/my_stufff/bin/hello.gcno -resource-dir /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/11.0.0 -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk -I/usr/local/include -stdlib=libc++ -internal-isystem /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/../include/c++/v1 -Wno-framework-include-private-from-public -Wno-atimport-in-framework-header -Wno-extra-semi-stmt -Wno-quoted-include-in-framework-header -fdeprecated-macro -fdebug-compilation-dir /Users/Elemental/Downloads/SDL2-2.0.12/my_stufff -ferror-limit 19 -fmessage-length 185 -stack-protector 1 -mdarwin-stkchk-strong-link -fblocks -fencode-extended-block-signature -fregister-global-dtors-with-atexit -fobjc-runtime=macosx-10.14.0 -fobjc-exceptions -fcxx-exceptions -fexceptions -fmax-type-align=16 -fdiagnostics-show-option -fcolor-diagnostics -x objective-c++

LD_TOOL = libtool

C_OUTPUT_SPECIFIER = -o

LIBS = -lc++ -lSystem /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/11.0.0/lib/darwin/libclang_rt.osx.a


INCLUDE_DIRS += -Imac_osx/include

TARGETS = cool.app/cool
