#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_opengl3.h"


ModuleEditor::ModuleEditor(){

}


ModuleEditor::~ModuleEditor(){

}

bool ModuleEditor::Init() {
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
	if (err){
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
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->glcontext);
	return true;
}

update_status ModuleEditor::PreUpdate() {
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update() {
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate() {
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditor::CleanUp(){

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}