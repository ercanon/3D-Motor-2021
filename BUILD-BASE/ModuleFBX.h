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
#include <assimp/Importer.hpp> 

class ModuleFBX: public Module
{
public:
    ModuleFBX(Application* app, bool start_enabled = true);

    ~ModuleFBX();

    bool Init();
    bool CleanUp();
    
    void LoadFbx(const char* file_path);

private:
    void InitFbx(const aiMesh* paiFbx, GameObject figure);
};
#endif
