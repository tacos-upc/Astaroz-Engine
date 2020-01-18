#pragma once
#include "Module.h"
#include "physfs-3.0.2/src/physfs.h"
#include <vector>
#include <string>

#define FILTERS ".fbx\0.png\0.ogg\0.mp3\0\0"
#define PATH_SEP '\\'
#define PATH_MAX 260

enum FileBrowsingMode
{
	FILE_BROWSING_LOAD,
	FILE_BROWSING_SAVE
};

struct PathStruct
{
	std::string path;
	std::string name;
	std::string ext;

	bool isOk;

	PathStruct()
	{
		path = "";
		name = "";
		isOk = false;
	}

	std::string fullpath()
	{
		return path == "" && name == ""? "" : "/" + path + "/" + name + ext;
	}
};

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem();
	~ModuleFileSystem();

	bool Init() override;
	bool Start() override;
	bool CleanUp() override;

	update_status Update() override;

	bool Exist(const char*);
	unsigned int Size(const char*); 
	bool CreateDir(const char*);
	bool IsDir(const char*);
	bool Delete(const char*);

	bool openFileBrowser(FileBrowsingMode);

	void save(const char*, const char*);
	char* load(const char*);

	const char* getWritePath();

private:
	const char* fileSystemPath;
	int searchPathPriority = 1;
	PathStruct selectedDir = PathStruct();
	std::vector<PathStruct> breadCrumbs;

	void mount(const char*);
	void enableWriteDir(const char*);
	std::vector<std::string> splitStringToVector(std::string, char, bool);
	PathStruct ParsePathFileName(const std::string& vPathFileName);
	bool replaceString(std::string& str, const std::string& oldStr, const std::string& newStr);
};

