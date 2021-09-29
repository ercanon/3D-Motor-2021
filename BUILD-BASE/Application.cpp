#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	imgui = new ModuleImGui(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	
	// Scenes
	AddModule(scene_intro);

	// ImGui
	AddModule(imgui);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	list <Module*>::iterator item = list_modules.end();

	while(*item != NULL)
	{
		delete *item; // mirar data que es en el p2list
		item = item--; // coger el previo de la lista
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	list <Module*>::iterator item = list_modules.begin();

	while(*item != NULL && ret == true)
	{
		ret = *item->Init();
		item = item++; //coger el siguiente de la lista
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while(*item != NULL && ret == true)
	{
		ret = *item->Start();
		item = item++;
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	list <Module*>::iterator item = list_modules.begin();
	
	while(*item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = *item->PreUpdate(dt);
		item = item++;
	}

	item = list_modules.begin();

	while(*item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = *item->Update(dt);
		item = item++;
	}

	item = list_modules.begin();

	while(*item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = *item->PostUpdate(dt);
		item = item++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	list <Module*>::iterator item = list_modules.end();

	while(*item != NULL && ret == true)
	{
		ret = *item->CleanUp();
		item = item--;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}