

bin/main.o: windows/src/main.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $< $(C_OUTPUT_SPECIFIER)$@
