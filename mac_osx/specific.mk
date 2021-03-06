
bin/main.o: mac_osx/src/main.mm
	@mkdir -p $(dir $@)
	@echo "Building $@"
	@$(CC) $(C_FLAGS) -D DYLIB_DIR=\""$(shell pwd)/bin/"\" $< $(C_OUTPUT_SPECIFIER)$@

bin/keyboard.o: mac_osx/src/keyboard.mm mac_osx/include/keyboard.h
	@mkdir -p $(dir $@)
	@echo "Building $@"
	@$(CC) $(C_FLAGS) $< $(C_OUTPUT_SPECIFIER)$@

bin/tower_d.dylib: bin/tower_main.o bin/video.o
	@mkdir -p $(dir $@)
	@echo "Linking $@"
	@$(LD) $(LD_DLL_FLAGS) -exported_symbol "_GameShutdown" -exported_symbol "_GameInit" -exported_symbol "_GameUpdate" $^ -o $@	

bin/blue_d.dylib: bin/blue_main.o
	@mkdir -p $(dir $@)
	@echo "Linking $@"
	@$(LD) $(LD_DLL_FLAGS) -exported_symbol "_GameShutdown" -exported_symbol "_GameInit" -exported_symbol "_GameUpdate" $^ -o $@	

cool: $(TARGET_OBJS) bin/main.o bin/keyboard.o
	@echo "Linking $(filter %.o, $^) -> $@"
	@$(LD) $(LD_FLAGS) $(LIBS) -o $@ $(filter %.o,$^) $(LIB_DIRS)

cool.app/cool: cool mac_osx/resources/Info.plist bin/tower_d.dylib bin/blue_d.dylib
	@echo "Creating Bundle $@"
	@mkdir -p cool.app/bin
	@cp mac_osx/resources/Info.plist cool.app/Info.plist
	@cp bin/tower_d.dylib cool.app/bin/tower_d.dylib
	@cp bin/blue_d.dylib cool.app/bin/blue_d.dylib
	@cp cool cool.app/cool
