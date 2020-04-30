


specific_cpps = keyboard_updates console_another performance app_window_helper

define SPECIFIC_BUILDER
bin/$(1).o: windows/src/$(1).cpp windows/include/$(1).h
	mkdir -p $$(dir $$@)
	$$(CC) $$(C_FLAGS) -c $$< $$(C_OUTPUT_SPECIFIER)$$@
SPECIFIC_OBJS += bin/$(1).o
endef

# // todo remove include after moving bitmap to c file.
bin/main.o: windows/src/main.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< $(C_OUTPUT_SPECIFIER)$@
SPECIFIC_OBJS += bin/main.o

$(foreach mod,$(specific_cpps),$(eval $(call SPECIFIC_BUILDER,$(mod))))

bin/tower_d.dll: bin/tower_main.o bin/console_another.o bin/video.o 
	$(LD) -DLL -EXPORT:GameInit -EXPORT:GameUpdate -EXPORT:GameShutdown /OUT:$@ $^

bin/blue_d.dll: bin/blue_main.o bin/blue_entity.o bin/console_another.o bin/video.o bin/game_memory.o bin/grid_helpers.o
	$(LD) -DLL -EXPORT:GameInit -EXPORT:GameUpdate -EXPORT:GameShutdown /OUT:$@ $^

cool.exe: $(SPECIFIC_OBJS) $(TARGET_OBJS) bin/tower_d.dll bin/blue_d.dll bin/lua.lib
	$(LD) /OUT:$@ $(LIBS) $(filter %.o,$^) 

bin/lua.lib: $(LUA_TARGET_OBJS)
	$(AR) $^ /OUT:$@

windows_child.exe: clanger/src/windows_child.cpp
	$(CC) clanger/src/windows_child.cpp

windows_parent.exe: clanger/src/windows_parent.cpp
	$(CC) clanger/src/windows_parent.cpp $(LIBS)

TARGETS += windows_child.exe
TARGETS += windows_parent.exe

