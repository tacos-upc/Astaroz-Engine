#include "ModuleFileSystem.h"
#include "physfs-3.0.2/src/physfs.h"
#include "SDL.h"
#include "Globals.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "IconsFontAwesome5.h"
#include <vector>
#include <string>


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
	mount(PHYSFS_getBaseDir());

	extensions.push_back("sav");

	filteredExtensions.push_back("ini");
	filteredExtensions.push_back("dll");
	filteredExtensions.push_back("exe");
	filteredExtensions.push_back("pdb");
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

update_status ModuleFileSystem::Update()
{
	return UPDATE_CONTINUE;
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

bool ModuleFileSystem::IsFile(const char* path)
{
	PHYSFS_Stat fileData;
	PHYSFS_stat(path, &fileData);

	return fileData.filetype == PHYSFS_FILETYPE_REGULAR;
}

bool ModuleFileSystem::Delete(const char* filePathAndName)
{
	return PHYSFS_delete(filePathAndName) != 0;
}

bool ModuleFileSystem::openFileBrowser(FileBrowsingMode mode)
{
	bool keepAlive = true;
	if (ImGui::Begin(mode == FILE_BROWSING_LOAD ? "Load..." : "Save...", &keepAlive, ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::Button(ICON_FA_FOLDER_PLUS " New Folder"))
		{

		}

		ImGui::SameLine();
		
		if (ImGui::Button("Root"))
		{
			selectedDir = PathStruct();
			breadCrumbs.clear();
		}

		ImGui::SameLine();
		ImGui::Text(": >");

		for (size_t i = 0; i < breadCrumbs.size(); i++)
		{
			ImGui::SameLine();
			if (ImGui::Button(breadCrumbs.at(i).name.c_str()))
			{
				selectedDir = breadCrumbs.at(i);
				int size = breadCrumbs.size();
				for (size_t j = i; j < size; j++)
				{
					breadCrumbs.pop_back();
				}
			}
		}

		ImGui::Separator();
		ImGui::Separator();

		char **files = PHYSFS_enumerateFiles(selectedDir.fullpath().c_str());
		char **i;
		if (files != nullptr)
		{
			for (i = files; *i != NULL; i++)
			{
				PathStruct pathStruct = PathStruct();
				pathStruct.name = *i;
				pathStruct.path = selectedDir.fullpath();

				if (IsFile(*i) && isAllowedForRender(*i))
				{
					if (ImGui::Button(*i))
					{
						if (IsDir(pathStruct.fullpath().c_str()))
						{
							selectedDir = pathStruct;
							breadCrumbs.push_back(pathStruct);
						}
						else if (IsFile(pathStruct.fullpath().c_str()))
						{
							pathStruct.ext = pathStruct.name.substr(pathStruct.name.find_last_of(".") + 1);
							selectedFile = pathStruct;
						}

					}
				}
			}
			PHYSFS_freeList(files);
		}

		if (isLoadableFile(selectedFile.ext))
		{
			ImGui::Separator();
			if (ImGui::Button("Load"))
			{

			}
		}

		ImGui::End();
	}

	if (!keepAlive)
	{
		//Cleanup
		selectedDir = PathStruct();
		selectedFile = PathStruct();
	}
	
	return keepAlive;
}

void ModuleFileSystem::save(const char* dir, const char* fileName)
{
}

char* ModuleFileSystem::load(const char* fullNameAndPath)
{
	PHYSFS_file* filePointer = nullptr;
	char* fileBuffer;

	if (Exist(fullNameAndPath))
	{
		filePointer = PHYSFS_openRead(fullNameAndPath);
		unsigned int size = Size(fullNameAndPath);
		fileBuffer = new char[size];
		PHYSFS_read(filePointer, fileBuffer, 1, size);
		PHYSFS_close(filePointer);
	}

	return fileBuffer;
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

std::vector<std::string> ModuleFileSystem::splitStringToVector(std::string text, char delimiter, bool pushEmpty)
{
	std::vector<std::string> arr;
	if (text.size() > 0)
	{
		std::string::size_type start = 0;
		std::string::size_type end = text.find(delimiter, start);
		while (end != std::string::npos)
		{
			std::string token = text.substr(start, end - start);
			if (token.size() > 0 || (token.size() == 0 && pushEmpty))
				arr.push_back(token);
			start = end + 1;
			end = text.find(delimiter, start);
		}
		arr.push_back(text.substr(start));
	}
	return arr;
}

PathStruct ModuleFileSystem::ParsePathFileName(const std::string& vPathFileName)
{
	PathStruct res;

	if (vPathFileName.size() > 0)
	{
		std::string pfn = vPathFileName;
		std::string separator(1u, PATH_SEP);
		replaceString(pfn, "\\", separator);
		replaceString(pfn, "/", separator);

		size_t lastSlash = pfn.find_last_of(separator);
		if (lastSlash != std::string::npos)
		{
			res.name = pfn.substr(lastSlash + 1);
			res.path = pfn.substr(0, lastSlash);
			res.isOk = true;
		}

		size_t lastPoint = pfn.find_last_of('.');
		if (lastPoint != std::string::npos)
		{
			if (!res.isOk)
			{
				res.name = pfn;
				res.isOk = true;
			}
			res.ext = pfn.substr(lastPoint + 1);
			replaceString(res.name, "." + res.ext, "");
		}
		
	}

	if (res.path == "" && res.name == ""); res.path = vPathFileName; res.name = vPathFileName;

	return res;
}

bool ModuleFileSystem::replaceString(std::string& str, const std::string& oldStr, const std::string& newStr)
{
	bool found = false;
	size_t pos = 0;
	while ((pos = str.find(oldStr, pos)) != ::std::string::npos)
	{
		found = true;
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
	return found;
}

bool ModuleFileSystem::isLoadableFile(std::string ext)
{
	bool canI = false;

	for (size_t i = 0; i < extensions.size(); i++)
	{
		if (ext == extensions.at(i).c_str())
		{
			canI |= true;
		}
	}

	return canI;
}

bool ModuleFileSystem::isAllowedForRender(const char* file)
{
	std::string str = std::string(file);
	const char* ext = str.substr(str.find_last_of(".") + 1).c_str();

	bool canI = true;

	for (size_t i = 0; i < filteredExtensions.size(); i++)
	{
		if (ext == filteredExtensions.at(i).c_str())
		{
			canI &= false;
		}
	}

	return canI;
}
