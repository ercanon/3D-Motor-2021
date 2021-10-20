#include "Application.h"
#include "ModuleFBX.h"
#include "Globals.h"

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#pragma comment (lib, "assimp-vc142-mt.lib")

ModuleFBX::ModuleFBX(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleFBX::~ModuleFBX()
{
}

bool ModuleFBX::Init()
{
	bool ret = true;

	return ret;
}

update_status ModuleFBX::PreUpdate(float dt) 
{

	return UPDATE_CONTINUE;
}

update_status ModuleFBX::Update(float dt) 
{

	return UPDATE_CONTINUE;
}

update_status ModuleFBX::PostUpdate(float dt) 
{

	return UPDATE_CONTINUE;
}

bool ModuleFBX::CleanUp() 
{
	bool ret = true;

	return ret;
}