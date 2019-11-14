#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"




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

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->glcontext);
	ImGui_ImplOpenGL3_Init();

	//flags to show windows
	show_demo_window = false;
	show_log_window = true;
	show_about_window = true;
	return true;
}

update_status ModuleEditor::PreUpdate() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update() {

	if (show_demo_window) {
		ImGui::ShowDemoWindow();
	}
	if (show_log_window) {
		ImGui::Begin("Logger Console", &show_log_window);
		//Menu


		//Console
		ImGui::TextUnformatted(myBuffer.begin());
		if (scrollToBottom)
			ImGui::SetScrollHere(1.0f);
		scrollToBottom = false;
		ImGui::End();
	}
	if (show_about_window) {
		//Menu
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		//About
		ImGui::Begin("About...", &show_about_window);
		ImGui::BulletText("Engine name: MyOwnEngine");
		ImGui::Text("This engine was performed in UPC master - game programming");
		ImGui::BulletText("Author: Pau Casas");
		ImGui::BulletText("Free license");
		ImGui::Text("Libraries:");
		ImGui::BulletText("SDL");
		ImGui::BulletText("OpenGL");
		ImGui::BulletText("MathGeoLib");
		ImGui::BulletText("ImGUI");		
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate() {
	
	//SDL_GL_SwapWindow(App->window->window);

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

void ModuleEditor::processEvent(SDL_Event event){
	ImGui_ImplSDL2_ProcessEvent(&event);
}

