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
	enableWriteDir(SDL_GetPrefPath("Astaroz", "AstarozEngine"));
	
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

unsigned int ModuleFileSystem::Size(const char* filePathAndName)
{
	unsigned int size = 0;

	if (Exist(filePathAndName))
	{
		PHYSFS_file* file = PHYSFS_openRead(filePathAndName);
		size = PHYSFS_fileLength(file);
		PHYSFS_close(file);
	}

	return size;
}

bool ModuleFileSystem::CreateDir(const char* dirName)
{
	return PHYSFS_mkdir(dirName) == 0;
}

bool ModuleFileSystem::IsDir(const char* path)
{
	PHYSFS_Stat fileData;
	PHYSFS_stat(path, &fileData);

	return fileData.filetype == PHYSFS_FILETYPE_DIRECTORY;
}

bool ModuleFileSystem::Delete(const char* filePathAndName)
{
	return PHYSFS_delete(filePathAndName) != 0;
}

void ModuleFileSystem::save(const char* dir, const char* fileName)
{
}

const char * ModuleFileSystem::getWritePath()
{
	return PHYSFS_getWriteDir();
}

void ModuleFileSystem::mount(const char* path)
{
	//path of the dir we want to add, "" <- Add under the root hierarchy
	PHYSFS_mount(path, "", searchPathPriority);
	searchPathPriority += 1;
}

void ModuleFileSystem::enableWriteDir(const char* writeDir)
{
	PHYSFS_setWriteDir(writeDir);
}
