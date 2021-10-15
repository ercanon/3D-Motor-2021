#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_opengl.h"
#include <vector>

using namespace std;

class ModuleEditor : public Module
{
public:
	
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	void Resize(int num);
	void LogToConsole();
	bool CleanUp();

public: 

	SDL_GLContext gl_context;
	ImGuiIO io_copy;

	ImVec4 clear_color;

	// Windows Editor
	bool configB = true;
	bool consoleB = true;
	bool aboutB = false;

	// Window config
	float bright;
	int width;
	int height;
	int posWindowX, posWindowY;
	int posX, posY;
	SDL_DisplayMode DM;

	bool fullscreen = false;
	bool resizable = true;
	bool borderless = true;
	bool fulldesktop = false;
	//bool resizeActive = false;

	// Framerate config
	std::vector<float> fps_log;
	std::vector<float> ms_log;

	// Input config
	ImGuiTextBuffer input;
	bool scroll;

	// Console
	ImGuiTextBuffer buffer;
	bool scrollC;

private:

	// Window config Helpers
	int modifywidth, modifyheight;
	int actualwidth, actualheight;
};

#endif