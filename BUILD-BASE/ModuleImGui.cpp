#include "Application.h"
#include "ModuleImGui.h"

ModuleImGui::ModuleImGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleImGui::~ModuleImGui()
{
}

bool ModuleImGui::Init()
{
	bool ret = true;
	return ret;
}

update_status ModuleImGui::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	bool ret = true;
	return ret;
}