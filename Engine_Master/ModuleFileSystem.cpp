#include "ModuleFileSystem.h"
#include "physfs-3.0.2/src/physfs.h"
#include "SDL.h"
#include "Globals.h"

ModuleFileSystem::ModuleFileSystem()
{
}


ModuleFileSystem::~ModuleFileSystem()
{
}

bool ModuleFileSystem::Init()
{
	PHYSFS_init(NULL);	
	addToSearchPath(SDL_GetPrefPath("Astaroz", "AstarozEngine"));
	
	return true;
}

bool ModuleFileSystem::Start()
{
	return true;
}

bool ModuleFileSystem::CleanUp()
{
	PHYSFS_deinit();
	return true;
}

bool ModuleFileSystem::Exist(const char* filePathAndName)
{
	return PHYSFS_exists(filePathAndName);
}

void ModuleFileSystem::addToSearchPath(const char* path)
{
	PHYSFS_addToSearchPath(path, searchPathPriority);
	searchPathPriority += 1;
}
