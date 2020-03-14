

bin/main.o: mac_osx/src/main.mm
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $< $(C_OUTPUT_SPECIFIER)$@
