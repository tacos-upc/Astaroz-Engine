#include "ModuleEditorCamera.h"
#include "Application.h"
#include "Geometry/Frustum.h"


ModuleEditorCamera::ModuleEditorCamera() {

}


ModuleEditorCamera::~ModuleEditorCamera() {

}

bool ModuleEditorCamera::Init() {
	
	return true;
}

update_status ModuleEditorCamera::PreUpdate() {

	return UPDATE_CONTINUE;
}

update_status ModuleEditorCamera::Update() {

	return UPDATE_CONTINUE;
}

update_status ModuleEditorCamera::PostUpdate() {

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditorCamera::CleanUp() {

	return true;
}