#pragma once
#include "Module.h"
class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem();
	~ModuleFileSystem();

	bool Init() override;
	bool Start() override;
	bool CleanUp() override;

	bool Exist(const char*);
	unsigned int Size(const char*); 
	bool CreateDir(const char*);
	bool IsDir(const char*);
	bool Delete(const char*);

	bool openFileBrowser();
	const char* selectFileInBrowser();

	void save(const char*, const char*);
	void load(const char*);

	const char* getWritePath();

private:
	const char* fileSystemPath;
	int searchPathPriority = 1;

	void mount(const char*);
	void enableWriteDir(const char*);
};

