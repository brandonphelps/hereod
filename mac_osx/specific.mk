



bin/main.o: mac_osx/src/main.mm
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $< $(C_OUTPUT_SPECIFIER)$@


cool: bin/main.o bin/video.o
	$(LD) $(LD_FLAGS) $(LIBS) -o $@ $^ $(LIB_DIRS)



