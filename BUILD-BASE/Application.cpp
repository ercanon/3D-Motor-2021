#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);
	primitive = new ModulePrimitives(this);
	texture = new ModuleTextures(this);
	fbx = new ModuleFBX(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);

	// ImGui - Editor
	AddModule(editor);

	// Primitives
	AddModule(primitive);
	AddModule(fbx);
	AddModule(texture);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	list <Module*>::iterator iterator = list_modules.end();
	iterator--;
	Module* item;

	while(iterator != list_modules.begin())
	{
		item = *iterator;
		iterator--;
		delete item;
	}

	item = *iterator;
	delete item;
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	list<Module*>::iterator iterator = list_modules.begin();
	Module* item;

	while(iterator != list_modules.end() && ret == true)
	{
		item = *iterator;
		ret = item->Init();
		iterator++;
	}

	// After all Init calls we call Start() in all modules
	editor->LOG("Application Start --------------");
	iterator = list_modules.begin();

	while(iterator != list_modules.end() && ret == true)
	{
		item = *iterator;
		ret = item->Start();
		iterator++;
	}
	
	ms_timer.Start();

	SDL_GetVersion(&SDLversion);
	cpuCountStart = SDL_GetCPUCount();
	cpuCacheSizeStart = SDL_GetCPUCacheLineSize();
	ramStart = SDL_GetSystemRAM() / 1024;
	
	if (SDL_HasAVX())
		capsStart += "AVX, ";
	if (SDL_HasAVX2())
		capsStart += "AVX2, ";
	if (SDL_HasMMX())
		capsStart += "MMX, ";
	if (SDL_HasRDTSC())
		capsStart += "RDTSC, ";
	if (SDL_HasSSE())
		capsStart += "SSE, ";
	if (SDL_HasSSE2())
		capsStart += "SSE2, ";
	if (SDL_HasSSE3())
		capsStart += "SSE3, ";
	if (SDL_HasSSE41())
		capsStart += "SSE41, ";
	if (SDL_HasSSE42())
		capsStart += "SSE42, ";

	// Erase last ", "
	int l = capsStart.size();
	capsStart.erase(l - 2, l - 1);

	gpuIntegratedModelStart = (const char*)glGetString(GL_RENDERER);
	gpuIntegratedVendorStart = (const char*)glGetString(GL_VENDOR);

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;

	Uint32 last_frame_ms = ms_timer.Read();
	float wait = (1000.f / (float)maxFPS) - (float)last_frame_ms;
	SDL_Delay(static_cast<Uint32>(fabs(wait)));

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
	
	list <Module*>::iterator iterator = list_modules.begin();
	Module* item;
	
	while(iterator != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		item = *iterator;
		ret = item->PreUpdate(dt);
		iterator++;
	}

	iterator = list_modules.begin();

	while(iterator != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		item = *iterator;
		ret = item->Update(dt);
		iterator++;
	}

	iterator = list_modules.begin();

	while(iterator != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		item = *iterator;
		ret = item->PostUpdate(dt);
		iterator++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	list <Module*>::iterator iterator = list_modules.end();
	Module* item;

	iterator--;

	while(iterator != list_modules.begin() && ret == true)
	{
		item = *iterator;
		ret = item->CleanUp();
		iterator--;
	}

	item = *iterator;
	ret = item->CleanUp();

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}