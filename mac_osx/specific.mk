



bin/main.o: mac_osx/src/main.mm
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $< $(C_OUTPUT_SPECIFIER)$@


bin/tower_d.dylib: bin/tower_main.o

cool: bin/main.o bin/video.o
	$(LD) $(LD_FLAGS) $(LIBS) -o $@ $^ $(LIB_DIRS)
