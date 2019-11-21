#include "ModuleEditor.h"


ModuleEditor::ModuleEditor()
{

}

ModuleEditor::~ModuleEditor()
{

}

bool ModuleEditor::Init()
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
	show_log_window = false;
	show_about_window = false;
	show_camera_window = false;

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
	//Menu
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("Demo window", NULL, &show_demo_window);
			ImGui::MenuItem("Logger console", NULL, &show_log_window);
			ImGui::MenuItem("Camera management", NULL, &show_camera_window);
			ImGui::EndMenu();
		};
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About", NULL, &show_about_window);
			ImGui::EndMenu();
		};
		ImGui::EndMainMenuBar();
	}
	//Demo flag
	if (show_demo_window) 
	{
		ImGui::ShowDemoWindow();
	}
	//Log flag
	if (show_log_window) 
	{
		ImGui::Begin("Logger Console", &show_log_window);
		ImGui::TextUnformatted(myBuffer.begin());
		if (scrollToBottom)
			ImGui::SetScrollHere(1.0f);
		scrollToBottom = false;
		ImGui::End();
	}
	//About flag
	if (show_about_window) 
	{
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
	//Camera flag
	if (show_camera_window) 
	{
		ImGui::Begin("Camera management", &show_camera_window);
		float fov = App->editorCamera->myFrustum.verticalFov;
		if (ImGui::SliderFloat("Change vertical FOV", &fov, 0.01f, 2.5f, "%.2f", 1.0f))
		{
			App->editorCamera->changeFOV(fov);
		}
		/*float position = App->editorCamera->myFrustum.pos.x;
		if (ImGui::SliderFloat("Position", &position, 0.01f, 20.0f, "%.2f", 1.0f))
		{
			App->editorCamera->changePosition(position);
		}*/
		ImGui::End();
	}

	//Draw
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
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

