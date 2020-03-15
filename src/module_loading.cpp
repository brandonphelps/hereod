
#include "game_module.h"

#ifdef _WIN32
#include <windows.h>
#include "console_another.h"
#else
#include <dlfcn.h>
#endif

void LoadModule(ModuleFunctions& module_funcs, const std::string& lib_path)
{
#ifdef _WIN32	
	module_funcs.lib_handle = LoadLibrary(lib_path.c_str());
#else
	module_funcs.lib_handle = dlopen(lib_path.c_str(), RTLD_LOCAL);
#endif

	if(!module_funcs.lib_handle)
	{
#ifdef _WIN32	
		WriteLine("Failed to load library ./shrug");
#else
		printf("Failed to load library %s\n", dlerror());
#endif
		return;
	}

#ifdef _WIN32
	module_funcs.GameInit = (FGameInit)GetProcAddress((HINSTANCE)module_funcs.lib_handle, "GameInit");
	module_funcs.GameUpdate = (FGameUpdate)GetProcAddress((HINSTANCE)module_funcs.lib_handle, "GameUpdate");
	module_funcs.GameShutdown = (FGameShutdown)GetProcAddress((HINSTANCE)module_funcs.lib_handle, "GameShutdown");
#else
	module_funcs.GameInit = (FGameInit)dlsym(module_funcs.lib_handle, "GameInit");
	module_funcs.GameUpdate = (FGameUpdate)dlsym(module_funcs.lib_handle, "GameUpdate");
	module_funcs.GameShutdown = (FGameShutdown)dlsym(module_funcs.lib_handle, "GameShutdown");
#endif

  if(module_funcs.GameInit == NULL || module_funcs.GameUpdate == NULL || module_funcs.GameShutdown == NULL)
  {
#ifdef _WIN32	
		WriteLine("Failed to get symbols");
#else
    printf("Failed to get symbols, %s\n", dlerror());
#endif

    module_funcs.GameInit = NULL;
    module_funcs.GameUpdate = NULL;
    module_funcs.GameShutdown = NULL;
  }
  else
  {
#ifdef _WIN32	
		WriteLine("Failed load module " + lib_path);
#else
	  printf("Succesfully loaded module\n");
#endif
  }
}


void UnloadModule(ModuleFunctions& module_funcs)
{
	if(module_funcs.lib_handle)
	{
#ifdef _WIN32

#else
		dlclose(module_funcs.lib_handle);
#endif
		module_funcs.GameInit = NULL;
		module_funcs.GameShutdown = NULL;
		module_funcs.GameUpdate = NULL;
	}
}
