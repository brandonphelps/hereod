
bin/main.o: windows/src/main.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< $(C_OUTPUT_SPECIFIER)$@

bin/console_another.o: windows/src/console_another.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< $(C_OUTPUT_SPECIFIER)$@


bin/tower_main.o: tower_d/src/tower_main.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< $(C_OUTPUT_SPECIFIER)$@

bin/tower_d.dll: bin/tower_main.o bin/console_another.o bin/video.o
	$(LD) -DLL -EXPORT:GameInit -EXPORT:GameUpdate -EXPORT:GameShutdown /OUT:$@ $^

cool.exe: bin/main.o bin/video.o bin/console_another.o bin/tower_d.dll
	$(LD) /OUT:$@ $(LIBS) $(filter %.o,$^) 



