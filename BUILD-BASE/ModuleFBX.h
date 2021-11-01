#ifndef __ModuleFBX_H__
#define __ModuleFBX_H__

#include "Module.h"
#include "Globals.h"
#include "Tools/List.h"
#include "glew.h"
#include "SDL_opengl.h"

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class ModuleFBX : public Module
{
public:
    ModuleFBX(Application* app, bool start_enabled = true);

    ~ModuleFBX();

    void LoadFbx(const char* file_path);

private:
    void InitMesh(GameObject figure, const aiMesh* paiMesh);
};
#endif
