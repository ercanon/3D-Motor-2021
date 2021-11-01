#pragma once

//#include "p2List.h"
#include <stdio.h>
#include <list>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePrimitives.h"
#include "ModuleEditor.h"
#include "ModuleTextures.h"
#include "ModuleFBX.h"

using namespace std;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ModulePrimitives* primitive;
	ModuleTextures* texture;
	ModuleFBX* fbx;

	// Framerate Config
	float maxFPS;

	// Hardware config
	SDL_version SDLversion;
	int cpuCountStart = 0;
	int cpuCacheSizeStart = 0;
	float ramStart = 0.0f;
	std::string capsStart = "";
	std::string gpuIntegratedModelStart = "";
	std::string gpuIntegratedVendorStart = "";

private:

	Timer	ms_timer;
	float	dt;
	list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};