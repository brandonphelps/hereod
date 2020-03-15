



bin/main.o: mac_osx/src/main.mm
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $< $(C_OUTPUT_SPECIFIER)$@


bin/tower_d.dylib: bin/tower_main.o bin/video.o
	mkdir -p $(dir $@)
	$(LD_TOOL) -dynamic $^ -o $@

cool: bin/main.o bin/video.o bin/tower_d.dylib
	$(LD) $(LD_FLAGS) $(LIBS) -o $@ $(filter %.o,$^) $(LIB_DIRS)
