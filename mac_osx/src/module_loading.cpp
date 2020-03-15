
#include "game_module.h"

#include <dlfcn.h>

void LoadModule(ModuleFunctions& module_funcs, const std::string& lib_path)
{
	module_funcs.lib_handle = dlopen(lib_path.c_str(), RTLD_LOCAL);
	if(!module_funcs.lib_handle)
	{
		printf("Failed to load library %s\n", dlerror());
		return;
	}

	module_funcs.GameInit = (FGameInit)dlsym(module_funcs.lib_handle, "GameInit");
	module_funcs.GameUpdate = (FGameUpdate)dlsym(module_funcs.lib_handle, "GameUpdate");
	module_funcs.GameShutdown = (FGameShutdown)dlsym(module_funcs.lib_handle, "GameShutdown");

  if(module_funcs.GameInit == NULL || module_funcs.GameUpdate == NULL || module_funcs.GameShutdown == NULL)
  {
    printf("Failed to get symbols, %s\n", dlerror());
    module_funcs.GameInit = NULL;
    module_funcs.GameUpdate = NULL;
    module_funcs.GameShutdown = NULL;
  }
  else
  {
	  printf("Succesfully loaded module\n");
  }
}


void UnloadModule(ModuleFunctions& module_funcs)
{
	if(module_funcs.lib_handle)
	{
		dlclose(module_funcs.lib_handle);
		module_funcs.GameInit = NULL;
		module_funcs.GameShutdown = NULL;
		module_funcs.GameUpdate = NULL;
	}
}
