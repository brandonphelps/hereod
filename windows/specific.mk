


specific_cpps = main keyboard_updates console_another performance

define SPECIFIC_BUILDER
bin/$(1).o: windows/src/$(1).cpp
	mkdir -p $$(dir $$@)
	$$(CC) $$(C_FLAGS) -c $$< $$(C_OUTPUT_SPECIFIER)$$@
SPECIFIC_OBJS += bin/$(1).o
endef

$(foreach mod,$(specific_cpps),$(eval $(call SPECIFIC_BUILDER,$(mod))))

bin/tower_d.dll: bin/tower_main.o bin/console_another.o bin/video.o 
	$(LD) -DLL -EXPORT:GameInit -EXPORT:GameUpdate -EXPORT:GameShutdown /OUT:$@ $^

bin/blue_d.dll: bin/blue_main.o bin/blue_entity.o bin/console_another.o bin/video.o bin/grid_helpers.o
	$(LD) -DLL -EXPORT:GameInit -EXPORT:GameUpdate -EXPORT:GameShutdown /OUT:$@ $^

cool.exe: $(SPECIFIC_OBJS) $(TARGET_OBJS) bin/tower_d.dll bin/blue_d.dll
	$(LD) /OUT:$@ $(LIBS) $(filter %.o,$^) 



