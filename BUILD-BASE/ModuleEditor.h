#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <vector>

using namespace std;

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

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
	bool CleanUp();
	void log(const char file[], int line, const char* format, ...);

public: 

	SDL_GLContext gl_context;
	ImGuiIO io_copy;

	ImVec4 clear_color;

	// Windows Editor
	bool hierarchyB = true;
	bool propertiesB = true;
	bool configB = false;
	bool consoleB = true;
	bool aboutB = false;

	// Window config
	float bright;
	int width;
	int height;
	int posX, posY;
	SDL_DisplayMode DM;

	bool fullscreen = false;
	bool resizable = true;
	bool rechangeR = false;
	bool borderless = true;
	bool fulldesktop = false;
	bool resizeActive = false;
	bool resizeActive2 = false;
	bool resizeActive3 = false;

	bool depthTest = true;
	bool cullFace = true;
	bool lighting = true;
	bool colorMaterial = true;
	bool texture = true;
	bool wireframe = false;
	bool fog = false;

	// Hierarchy config
	int figNum = 0;
	bool displayinfo = false;

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