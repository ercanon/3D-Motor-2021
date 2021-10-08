#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_opengl.h"

class ModuleEditor : public Module
{
public:
	
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public: 

	SDL_GLContext gl_context;
	ImGuiIO io_copy;

	bool show_demo_window;
	bool show_another_window;
	ImVec4 clear_color;
};

#endif