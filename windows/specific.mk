

bin/main.o: windows/src/main.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< $(C_OUTPUT_SPECIFIER)$@

bin/console_another.o: windows/src/console_another.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< $(C_OUTPUT_SPECIFIER)$@

cool.exe: bin/main.o bin/video.o bin/console_another.o
	$(LD) /OUT:$@ $(LIBS) $^ 



