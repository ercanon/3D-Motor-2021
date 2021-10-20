#ifndef __ModuleFBX_H__
#define __ModuleFBX_H__

#include "Module.h"

class ModuleFBX : public Module
{
public:
	ModuleFBX(Application* app, bool start_enabled = true);
	~ModuleFBX();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

};

#endif

