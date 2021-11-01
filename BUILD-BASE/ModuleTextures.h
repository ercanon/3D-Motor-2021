#ifndef __ModuleTextures_H__
#define __ModuleTextures_H__

#include "Module.h"
#include "Globals.h"
#include "Tools/List.h"
#include "glew.h"
#include "SDL_opengl.h"
#include "ilu.h"
#include "ilut.h"

class Material
{
public:
	GLuint textureID = -1;
	GLuint buff_texture;

	int texWidth;
	int texHeight;
};

class ModuleTextures : public Module
{
public: 
	ModuleTextures(Application* app, bool start_enabled = true);
	~ModuleTextures();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void LoadTextureBase();
	void LoadTextures(const char* file_path);

public:
	List<Material> listMaterials;
};
#endif
