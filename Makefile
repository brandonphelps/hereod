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
bin/$(1).o: src/$(1).cpp
	@mkdir -p bin
	@$$(CC) $$(C_FLAGS) $$(C_BIN_SPECIFIER) $$< $$(C_OUTPUT_SPECIFIER)$$@
TARGET_OBJS += bin/$(1).o

endef

bin/tower_main.o: tower_d/src/tower_main.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(C_FLAGS) $(C_BIN_SPECIFIER) $< $(C_OUTPUT_SPECIFIER)$@

bin/blue_main.o: blue_d/src/blue_main.cpp
	@mkdir -p $(dir $@)
	@echo "Building $@"
	@$(CC) $(C_FLAGS) $(C_BIN_SPECIFIER) $< $(C_OUTPUT_SPECIFIER)$@

# Common sources, i.e not module specific
$(eval $(call CommonSrcRule,module_loading))
$(eval $(call CommonSrcRule,video))
$(eval $(call CommonSrcRule,game_controller))
$(eval $(call CommonSrcRule,game_state))
$(eval $(call CommonSrcRule,grid_helpers))

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
