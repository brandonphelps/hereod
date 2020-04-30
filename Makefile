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
INCLUDE_DIRS += -Ibitmap/include
INCLUDE_DIRS += -Iconsole/include


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
$(eval $(call CommonSrcRule,sprite_sheet))
$(eval $(call CommonSrcRule,pixel_color))

define ModuleCommonRule
bin/$(2)/$(1).o: $(2)/src/$(1).cpp
	@mkdir -p bin/$(2)
	@$$(CC) $$(C_FLAGS) $$(C_BIN_SPECIFIER) $$< $$(C_OUTPUT_SPECIFIER)$$@
TARGET_OBJS += bin/$(2)/$(1).o
endef


$(eval $(call ModuleCommonRule,herod_bitmap,bitmap))
$(eval $(call ModuleCommonRule,herod_console,console))
$(eval $(call ModuleCommonRule,clanger,clanger))
$(eval $(call ModuleCommonRule,windows_exec,clanger))

LUA_TARGET_OBJS = 

define LuaModuleRule
bin/lua/$(1).o: lua/src/$(1).c
	@mkdir -p bin/lua
	@$$(CC) $$(C_FLAGS) $$(C_BIN_SPECIFIER) $$< $$(C_OUTPUT_SPECIFIER)$$@
LUA_TARGET_OBJS += bin/lua/$(1).o
endef


LUA_SRCS = lapi lauxlib lbaselib lbitlib lcode lcorolib lctype ldblib ldebug ldo ldump lfunc lgc linit liolib llex lmathlib lmem loadlib lobject lopcodes lparser lstate lstring lstrlib ltable ltablib ltm lundump lutf8lib lvm lzio 

$(foreach lsrc,$(LUA_SRCS),$(eval $(call LuaModuleRule,$(lsrc))))


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
