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


INCLUDE_DIRS += -Iinclude

LIB_DIRS = -L/usr/local/lib


C_FLAGS += $(INCLUDE_DIRS)

all: $(TARGETS)

define CommonSrcRule
bin/controller.o: src/controller.cpp
	mkdir -p $(dir $$@)
	$$(CC) $$(C_FLAGS) $$(C_BIN_SPECIFIER) $$< $$(C_OUTPUT_SPECIFIER)$$@

cool.exe: bin/$(1).o
endef


bin/video.o: src/video.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $(C_BIN_SPECIFIER) $< $(C_OUTPUT_SPECIFIER)$@

bin/tower_main.o: tower_d/src/tower_main.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $(C_BIN_SPECIFIER) $< $(C_OUTPUT_SPECIFIER)$@

bin/module_loading.o: src/module_loading.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $(C_BIN_SPECIFIER) $< $(C_OUTPUT_SPECIFIER)$@


$(eval $(call CommonSrcRule,controller))


bin/blue_main.o: blue_d/src/blue_main.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $(C_BIN_SPECIFIER) $< $(C_OUTPUT_SPECIFIER)$@

clean:
	rm -rf bin $(TARGETS)

print_predefines:
	$(CC) -dM -E -x c++ - < /dev/null







ifeq ($(OS),Windows_NT)

include windows/specific.mk

else

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)

include mac_osx/specific.mk

endif

endif
