#pragma once
#include "Module.h"
//#include "p2DynArray.h"
#include "Globals.h"

class ModuleScene3D : public Module
{
public:
	ModuleScene3D(Application* app, bool start_enabled = true);
	~ModuleScene3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:
};
