#include "ModuleEditor.h"
#include "windows.h"
#include "version.h"
#include "IL/il.h"
#include "IconsFontAwesome5.h"
#include "IconsFontAwesome5Brands.h"
#include "IconsFontAwesome5.h"
#include "ModuleTime.h"
#include "ModuleScene.h"
#include "ModuleEditorCamera.h"

ModuleEditor::ModuleEditor()
{

}

ModuleEditor::~ModuleEditor()
{

}

bool ModuleEditor::Init()
{
	//flags to show windows
	show_about_window = false;
	show_configuration_window = false;
	openComponentsMenu = false;
	return true;
}

bool ModuleEditor::Start()
{
	// Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = gladLoadGL() == 0;
#else
	bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
	if (err) {
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->glcontext);
	ImGui_ImplOpenGL3_Init();

	loadIcons();

	return true;
}

update_status ModuleEditor::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
	drawMainMenu();
	drawHierarchyPanel();
	drawCameraPanel();
	drawInspectorPanel(); 
	drawLogPanel();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
	//Draw
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleEditor::processEvent(SDL_Event event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void ModuleEditor::loadIcons()
{
	ImGuiIO& io = ImGui::GetIO();

	static const ImWchar icons[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig config;
	config.MergeMode = true;
	config.PixelSnapH = true;

	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("./fonts/" FONT_ICON_FILE_NAME_FAS, 12.f, &config, icons);

	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("./fonts/" FONT_ICON_FILE_NAME_FAR, 12.f, &config, icons);

	io.Fonts->AddFontDefault();
	static const ImWchar fabIcons[] = { ICON_MIN_FAB, ICON_MAX_FAB, 0 };
	io.Fonts->AddFontFromFileTTF("./fonts/" FONT_ICON_FILE_NAME_FAB, 12.f, &config, fabIcons);
}

void ModuleEditor::drawMainMenu()
{
	//Menu
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem(ICON_FA_SAVE " Save Scene"))
			{
				Serialization sceneSerial;
				App->scene->OnSave(sceneSerial);
				std::string serializedScene;
				sceneSerial.GetSerializedScene(serializedScene);

				App->scene->sceneSerialized = serializedScene;	//test

				//App->filesystem->Save(path.c_str(), serializedScene.c_str(), serializedScene.size() + 1);
			}
			if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Load Scene"))
			{
				//size_t readed_bytes;
				//char* scene_file_data = App->filesystem->Load(path.c_str(), readed_bytes);
				//std::string serialized_scene_string = scene_file_data;
				//free(scene_file_data);

				//Serialization sceneSerial(App->scene->sceneSerialized);
				//App->scene->OnLoad(sceneSerial);
				LOG(App->scene->sceneSerialized.c_str());
			}
			ImGui::EndMenu();
		};
		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("Configuration", NULL, &show_configuration_window);
			ImGui::EndMenu();
		};
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About", NULL, &show_about_window);
			ImGui::EndMenu();
		};
		ImGui::EndMainMenuBar();
	}
	//About flag
	if (show_about_window)
	{
		ImGui::Begin("About...", &show_about_window);
		ImGui::BulletText("Engine name: Astaroz engine");
		ImGui::Text("This engine was performed in UPC master - game programming");
		ImGui::BulletText("Author: Pau Casas");
		ImGui::BulletText("MIT License:"); ImGui::SameLine();
		if (ImGui::SmallButton("License"))
		{
			ShellExecuteA(nullptr, "open", "https://github.com/Pacasasgar/Astaroz-Engine/blob/master/LICENSE", nullptr, nullptr, SW_SHOWNORMAL);
		}
		ImGui::Separator();
		ImGui::Text("Github repository:"); ImGui::SameLine();
		if (ImGui::SmallButton("Astaroz engine link github"))
		{
			ShellExecuteA(nullptr, "open", "https://github.com/Pacasasgar/Astaroz-Engine", nullptr, nullptr, SW_SHOWNORMAL);
		}
		ImGui::End();
	}
	//Configuration flag
	if (show_configuration_window)
	{
		ImGui::Begin("Configuration", &show_configuration_window);

		//FPS
		fps_log.push_back(ImGui::GetIO().Framerate);
		if (fps_log.size() > 25) //Divides graph by sections like in PDF image
		{
			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			fps_log.erase(fps_log.begin());
		}

		//Memory consumption
		if (ImGui::TreeNode("Memory consumption"))
		{
			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
			DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
			DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
			DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
			ImGui::Text("Total virtual memory: %u KB", totalVirtualMem / 1024);
			ImGui::Text("Virtual memory used: %u KB", virtualMemUsed / 1024);
			ImGui::Separator();
			ImGui::Text("Total physical memory: %u KB", totalPhysMem / 1024);
			ImGui::Text("Physical memory used: %u KB", physMemUsed / 1024);
			ImGui::TreePop();
			ImGui::Separator();
		}

		//Hardware detection
		if (ImGui::TreeNode("Hardware detection"))
		{
			ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
			ImGui::Text("CPU cache line size : %d B", SDL_GetCPUCacheLineSize());
			ImGui::Separator();
			ImGui::Text("Total system RAM : %d MB", SDL_GetSystemRAM());
			ImGui::Separator();
			ImGui::Text("GPU: %s", glGetString(GL_RENDERER));
			ImGui::Text("GPU company: %s", glGetString(GL_VENDOR));
			ImGui::TreePop();
			ImGui::Separator();
		}

		//Software versions
		if (ImGui::TreeNode("Software versions"))
		{
			SDL_version sdlVersion;
			SDL_GetVersion(&sdlVersion);
			ImGui::BulletText("SDL (version %d.%d.%d)", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);
			ImGui::BulletText("Imgui (version %s)", ImGui::GetVersion());
			ImGui::BulletText("MathGeoLib");
			ImGui::BulletText("glew (version %s)", glewGetString(GLEW_VERSION));
			ImGui::BulletText("Assimp (version %d.%d.%d)", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());
			ImGui::BulletText("DevIL (version %d)", IL_VERSION);
			ImGui::TreePop();
			ImGui::Separator();
		}
		ImGui::End();
	}
}

void ModuleEditor::drawHierarchyPanel()
{
	ImGui::SetNextWindowSize(ImVec2(App->window->width * 0.2f, App->window->height * 0.65f));
	ImGui::SetNextWindowPos(ImVec2(0.0f, 50.0f));
	if (ImGui::Begin("Left panel", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		if (ImGui::BeginTabBar("", ImGuiTabBarFlags_FittingPolicyScroll))
		{
			//Hierarchy tab
			if (ImGui::BeginTabItem(ICON_FA_SITEMAP " Hierarchy"))
			{
				App->scene->drawHierarchy();

				ImGui::EndTabItem();
			}
			//Settings tab
			if (ImGui::BeginTabItem(ICON_FA_COG " Engine Settings"))
			{
				if (ImGui::CollapsingHeader(ICON_FA_CLOCK " Time"))
				{
					App->time->drawTimeData();
					ImGui::Separator();
				}
				if (ImGui::CollapsingHeader(ICON_FA_CAMERA_RETRO " Scene"))
				{
					App->renderer->drawSceneRenderSettings();
					ImGui::Separator();
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}

void ModuleEditor::drawCameraPanel()
{
	ImGui::SetNextWindowSize(ImVec2(App->window->width * 0.6f, App->window->height * 0.65f));
	ImGui::SetNextWindowPos(ImVec2(App->window->width * 0.2f, 50.0f));
	if (ImGui::Begin("Scene", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		if (ImGui::BeginTabBar("", ImGuiTabBarFlags_FittingPolicyScroll))
		{
			//Settings tab
			if (ImGui::BeginTabItem(ICON_FA_FEATHER_ALT " Scene"))
			{
				App->renderer->drawSceneView();
				ImGui::EndTabItem();
			}

			//HIierarchy tab
			if (ImGui::BeginTabItem(ICON_FA_GAMEPAD " Game"))
			{
				App->renderer->drawGameView();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}

void ModuleEditor::drawInspectorPanel()
{
	float componentsMenuY = 0;

	ImGui::SetNextWindowSize(ImVec2(App->window->width * 0.2f, App->window->height));
	ImGui::SetNextWindowPos(ImVec2(App->window->width * 0.8f, 50.0f));
	if (ImGui::Begin(ICON_FA_GLASSES " Inspector", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
	{
		GameObject* obj = App->scene->selectedByHierarchy;
		if (obj != nullptr && obj != App->scene->getRoot())
		{
			obj->DrawInspector();

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
			ImGui::Separator();
			ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.2f);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
			if (ImGui::Button("Add Component", ImVec2(ImGui::GetWindowSize().x * 0.6f, 25.0f)))
			{
				openComponentsMenu = true;
			}
			componentsMenuY = ImGui::GetCursorPosY() + 40.f;
		}
		ImGui::End();
		drawComponentsMenu(componentsMenuY);

	}
}

void ModuleEditor::drawComponentsMenu(float y)
{
	if (openComponentsMenu)
	{
		ImGui::SetNextWindowPos(ImVec2(App->window->width * 0.8f, y));
		ImGui::SetNextWindowSize(ImVec2(App->window->width * 0.2f, 200.f));
		if (ImGui::Begin("Components", &openComponentsMenu, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
		{
			for (size_t i = 0; i < ComponentType::COMPONENT_TYPE_COUNT; i++)
			{
				ImGui::SetNextItemWidth(App->window->width * 0.2f - 5.f);
				if (ImGui::Button(Component::typeToString(i)))
				{
					App->scene->selectedByHierarchy->CreateComponent((ComponentType)i);
					openComponentsMenu = false;
				}
			}
			if (!ImGui::IsWindowFocused()) openComponentsMenu = false;
			ImGui::End();
		}
	}
}

void ModuleEditor::drawLogPanel()
{
	ImGui::SetNextWindowSize(ImVec2(App->window->width * 0.8f, App->window->height * 0.3f));
	ImGui::SetNextWindowPos(ImVec2(0, 50.0f + App->window->height * 0.65f));

	if (ImGui::Begin(ICON_FA_TAPE " Logs", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
	{
		ImGui::TextUnformatted(myBuffer.begin());
		if (scrollToBottom)
			ImGui::SetScrollHere(1.0f);
		scrollToBottom = false;
		ImGui::End();
	}
}