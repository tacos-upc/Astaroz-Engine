#include "ModuleEditorCamera.h"
#include "ModuleModelLoader.h"


ModuleEditorCamera::ModuleEditorCamera()
{}


ModuleEditorCamera::~ModuleEditorCamera()
{}

bool ModuleEditorCamera::Init() 
{
	//Init variables
	multiplierMovSpeed = 1.f;
	rotSpeed = 200.f;
	zoomSpeed = 1.5f;
	pitch = 0.f;
	yaw = -90.f;
	radius = 0.f;
	orbitX = 0.f;
	orbitY = 0.f;
	allowMovement = false;

	//Frustum
	myFrustum.type = FrustumType::PerspectiveFrustum;
	myFrustum.pos = float3::zero;
	myFrustum.front = -float3::unitZ;
	myFrustum.up = float3::unitY;
	myFrustum.nearPlaneDistance = 0.1f;
	myFrustum.farPlaneDistance = 100.0f;
	myFrustum.verticalFov = math::pi / 4.0f;
	myFrustum.horizontalFov = 2.f * atanf(tanf(myFrustum.verticalFov * 0.5f) * calculateAspectRatio());

	//Matrices
	proj = myFrustum.ProjectionMatrix();
	model = float4x4::FromTRS(float3(0.0f, 0.0f, -4.0f), float3x3::RotateY(math::pi / 4.0f), float3(1.0f, 1.0f, 1.0f));
	//view = LookAt(myFrustum.pos, myFrustum.pos + myFrustum.front, myFrustum.up); --> Done in PostUpdate, not needed to do it here

	return true;
}

update_status ModuleEditorCamera::PreUpdate() 
{
	return UPDATE_CONTINUE;
}

update_status ModuleEditorCamera::Update()
{
	//Grid
	glLineWidth(1.0f);
	float d = 200.0f;
	glBegin(GL_LINES);
	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();
	
	//Axis
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	// red X
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
	// green Y
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);
	// blue Z
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
	glEnd();
	glLineWidth(1.0f);

	//Speed
	if (App->input->isKeyDown(SDL_SCANCODE_LSHIFT))
		multiplierMovSpeed = 2.0f;
	else
		multiplierMovSpeed = 1.0f;

	//Focus model
	if (App->input->isKeyDown(SDL_SCANCODE_F))
		focusModel();
	
	//Movement
	if (allowMovement)
	{
		if (App->input->isKeyDown(SDL_SCANCODE_A))
			changePositionX(-0.05f);
		if (App->input->isKeyDown(SDL_SCANCODE_D))
			changePositionX(0.05f);
		if (App->input->isKeyDown(SDL_SCANCODE_W))
			changePositionZ(0.05f);
		if (App->input->isKeyDown(SDL_SCANCODE_S))
			changePositionZ(-0.05f);
		if (App->input->isKeyDown(SDL_SCANCODE_Q))
			changePositionY(-0.05f);
		if (App->input->isKeyDown(SDL_SCANCODE_E))
			changePositionY(0.05f);
	}

	return UPDATE_CONTINUE;
}

update_status ModuleEditorCamera::PostUpdate() 
{
	proj = myFrustum.ProjectionMatrix();

	//Update view matrix in order to update camera position
	view = LookAt(myFrustum.pos, myFrustum.pos + myFrustum.front, myFrustum.up);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditorCamera::CleanUp() 
{
	return true;
}

float4x4 ModuleEditorCamera::LookAt(float3 eye, float3 target, float3 up) 
{
	float4x4 matrix;
	math::float3 f(target - eye);
	f.Normalize();
	math::float3 s(f.Cross(up));
	s.Normalize();
	math::float3 u(s.Cross(f));
	matrix[0][0] = s.x;
	matrix[0][1] = s.y;
	matrix[0][2] = s.z;
	matrix[1][0] = u.x;
	matrix[1][1] = u.y;
	matrix[1][2] = u.z;
	matrix[2][0] = -f.x;
	matrix[2][1] = -f.y;
	matrix[2][2] = -f.z;
	matrix[0][3] = -s.Dot(eye);
	matrix[1][3] = -u.Dot(eye);
	matrix[2][3] = f.Dot(eye);
	matrix[3][0] = 0.0f;
	matrix[3][1] = 0.0f;
	matrix[3][2] = 0.0f;
	matrix[3][3] = 1.0f;

	return matrix;
}

void ModuleEditorCamera::changeFOV(float fov)
{
	//Change FOV
	myFrustum.verticalFov = fov;
	myFrustum.horizontalFov = 2.f * atanf(tanf(myFrustum.verticalFov * 0.5f) * calculateAspectRatio());

	//Reasign projection matrix
	proj = myFrustum.ProjectionMatrix();
}

float ModuleEditorCamera::calculateAspectRatio()
{
	return (float) App->window->width / App->window->height;
}

void ModuleEditorCamera::setAspectFrustum()
{
	myFrustum.horizontalFov = 2.0f * atanf(tanf(myFrustum.verticalFov * 0.5f) * calculateAspectRatio());
	proj = myFrustum.ProjectionMatrix();
}

void ModuleEditorCamera::focusModel()
{
	//Get the info from the model BoundingBox
	float3 halfSize = App->modelLoader->myBoundingBox.HalfSize();
	float distX = halfSize.x / tanf(myFrustum.horizontalFov * 0.5f);
	float distY = halfSize.y / tanf(myFrustum.verticalFov * 0.5f);
	float camDist = MAX(distX, distY) + halfSize.z;
	float3 center = App->modelLoader->myBoundingBox.FaceCenterPoint(5);

	//Change camera position depending on the model
	myFrustum.pos = center + float3(0, 0, camDist);
	myFrustum.front = float3(0, 0, -1);
}

void ModuleEditorCamera::zoom(const float wheelY)
{
	if (wheelY > 0)
	{
		myFrustum.pos += myFrustum.front * zoomSpeed;	//Zoom in
	}
	else if (wheelY < 0)
	{
		myFrustum.pos -= myFrustum.front * zoomSpeed;	//Zoom out
	}
}

void ModuleEditorCamera::changePositionX(const float position)
{
	myFrustum.pos += position * multiplierMovSpeed * myFrustum.WorldRight();
}

void ModuleEditorCamera::changePositionY(const float position)
{
	myFrustum.pos += position * multiplierMovSpeed * myFrustum.up;
}

void ModuleEditorCamera::changePositionZ(const float position)
{
	myFrustum.pos -= position * multiplierMovSpeed * myFrustum.WorldRight().Cross(float3(0,1,0));
}

void ModuleEditorCamera::rotate(const float mouseMotionX, const float mouseMotionY)
{
	//Pitch
	float timePitch = mouseMotionY * rotSpeed;
	if (timePitch < 0)
		pitch = MAX(-89, pitch - timePitch);	//Limit rotation angles for pitch so we avoid square angles and upside down rotation
	else
		pitch = MIN(89, pitch - timePitch);

	//Yaw
	yaw += mouseMotionX * rotSpeed;

	//Apply rotation values to frustum
	myFrustum.front.x = cos(DegToRad(yaw)) * cos(DegToRad(pitch));
	myFrustum.front.y = sin(DegToRad(pitch));
	myFrustum.front.z = sin(DegToRad(yaw)) * cos(DegToRad(pitch));
	myFrustum.front.Normalize();

	//Update projection and view matrix from our updated frustum
	proj = myFrustum.ProjectionMatrix();
	view = myFrustum.ViewMatrix();
}

void ModuleEditorCamera::orbit(const float mouseMotionX, const float mouseMotionY)
{
	//Update orbit values on X and Y axis
	orbitX += mouseMotionX * rotSpeed;
	orbitY = MIN(89, orbitY + mouseMotionY * rotSpeed);

	//Update radius needed to apply orbit values to frustum
	radius = App->modelLoader->myBoundingBox.CenterPoint().Distance(myFrustum.pos);

	//Apply orbit values to frustum
	myFrustum.pos.x = cos(math::DegToRad(orbitX)) * cos(math::DegToRad(orbitY)) * radius;
	myFrustum.pos.y = sin(math::DegToRad(orbitY)) * radius;
	myFrustum.pos.z = sin(math::DegToRad(orbitX)) * cos(math::DegToRad(orbitY)) * radius;
	myFrustum.pos += App->modelLoader->myBoundingBox.CenterPoint();
	myFrustum.front = (App->modelLoader->myBoundingBox.CenterPoint() - myFrustum.pos).Normalized();

	//Update Yaw and Pitch (also used on rotation)
	yaw = math::RadToDeg(atan2(myFrustum.front.z, myFrustum.front.x));
	pitch = math::RadToDeg(asin(myFrustum.front.y));
}