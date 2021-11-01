#ifndef __ModulePrimitives_H__
#define __ModulePrimitives_H__

#include "Module.h"
#include "Globals.h"
#include "Tools/glmath.h"
#include "glew.h"
#include "SDL_opengl.h"
#include "Tools/List.h"
#include "ModuleTextures.h"

#include "Librerias/MathGeoLib/Math/float3.h"
#include "Librerias/MathGeoLib/Math/float4x4.h"
#include "Librerias/MathGeoLib/Math/Quat.h"

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

enum Shape
{
	CUBE,
	PYRAMID,
	FBX
};

class GameObject
{
public:
	bool active = false;
	std::string name;
	std::string shapename;

	Shape shape;

	uint buff_vertices;
	uint num_vertices;

	uint buff_indices;
	uint num_indices;

	uint buff_uvs;
	uint num_uvs;

	Material currentMaterial;

	float4x4 globalMatrix = float4x4::identity;
	float4x4 localMatrix = float4x4::identity;
	float4x4 localTransform = float4x4::identity;
	float3 position = {0, 0, 0};
	float3 eulerRot = {0, 0, 0};
	Quat rotation = {0, 0, 0, 1};
	float3 scale = {1, 1, 1};
};

class ModulePrimitives : public Module
{
public: 
	ModulePrimitives(Application* app, bool start_enabled = true);
	~ModulePrimitives();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();
	
	void CreateBuffer(Shape shape);
	void Draw(GameObject figure);

private:
	// Buffers
	GameObject BufferQuad(GameObject figure);
	GameObject BufferPyramid(GameObject figure);

public:
	List<GameObject> listFigures;
	int shapeCounter[3] = { 0, 0, 0 };
};
#endif
