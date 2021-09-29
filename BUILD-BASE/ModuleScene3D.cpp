#include "Globals.h"
#include "Application.h"
#include "ModuleScene3D.h"
#include "Primitive.h"

ModuleScene3D::ModuleScene3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleScene3D::~ModuleScene3D()
{}

// Load assets
bool ModuleScene3D::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	return ret;
}

// Load assets
bool ModuleScene3D::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleScene3D::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return UPDATE_CONTINUE;
}

