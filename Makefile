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


INCLUDE_DIRS += -Iinclude -Ilua/include -Iclanger/include

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
	@$(CC) $(C_FLAGS) -Iblue_d/include $(C_BIN_SPECIFIER) $< $(C_OUTPUT_SPECIFIER)$@

bin/blue_entity.o: blue_d/src/blue_entity.cpp blue_d/include/blue_entity.h
	@mkdir -p $(dir $@)
	@echo "Building $@"
	@$(CC) $(C_FLAGS) -Iblue_d/include $(C_BIN_SPECIFIER) $< $(C_OUTPUT_SPECIFIER)$@

# Common sources, i.e not module specific
$(eval $(call CommonSrcRule,module_loading))
$(eval $(call CommonSrcRule,video))
$(eval $(call CommonSrcRule,game_controller))
$(eval $(call CommonSrcRule,game_memory))
$(eval $(call CommonSrcRule,game_state))
$(eval $(call CommonSrcRule,game_recording))
$(eval $(call CommonSrcRule,grid_helpers))


define LuaCommonRule
bin/lua/$(1).o: lua/src/$(1).cpp
	@mkdir -p bin/lua
	@$$(CC) $$(C_FLAGS) $$(C_BIN_SPECIFIER) $$< $$(C_OUTPUT_SPECIFIER)$$@
TARGET_OBJS += bin/lua/$(1).o
endef


define BitMapCommonRule
bin/bitmap/$(1).o: bitmap/src/$(1).cpp
	@mkdir -p bin/bitmap
	@$$(CC) $$(C_FLAGS) $$(C_BIN_SPECIFIER) $$< $$(C_OUTPUT_SPECIFIER)$$@
TARGET_OBJS += bin/bitmap/$(1).o

endef
INCLUDE_DIRS += -Ibitmap/include


# $(eval $(call LuaCommonRule,lstate))
# $(eval $(call LuaCommonRule,lapi))


define ClangerCommonRule
bin/clanger/$(1).o: clanger/src/$(1).cpp
	@mkdir -p bin/clanger
	@$$(CC) $$(C_FLAGS) $$(C_BIN_SPECIFIER) $$< $$(C_OUTPUT_SPECIFIER)$$@
TARGET_OBJS += bin/clanger/$(1).o
endef


# $(eval $(call LuaCommonRule,lstate))
# $(eval $(call LuaCommonRule,lapi))

$(eval $(call ClangerCommonRule,clanger))
$(eval $(call ClangerCommonRule,windows_exec))



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
