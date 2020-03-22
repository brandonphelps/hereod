
specific_objs = bin/main.o bin/keyboard_updates.o bin/console_another.o

bin/main.o: windows/src/main.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< $(C_OUTPUT_SPECIFIER)$@

bin/keyboard_updates.o: windows/src/keyboard_updates.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< $(C_OUTPUT_SPECIFIER)$@


bin/console_another.o: windows/src/console_another.cpp
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< $(C_OUTPUT_SPECIFIER)$@

bin/tower_d.dll: bin/tower_main.o bin/console_another.o bin/video.o 
	$(LD) -DLL -EXPORT:GameInit -EXPORT:GameUpdate -EXPORT:GameShutdown /OUT:$@ $^

bin/blue_d.dll: bin/blue_main.o bin/console_another.o bin/video.o bin/grid_helpers.o
	$(LD) -DLL -EXPORT:GameInit -EXPORT:GameUpdate -EXPORT:GameShutdown /OUT:$@ $^

cool.exe: $(specific_objs) $(TARGET_OBJS) bin/tower_d.dll bin/blue_d.dll
	$(LD) /OUT:$@ $(LIBS) $(filter %.o,$^) 



