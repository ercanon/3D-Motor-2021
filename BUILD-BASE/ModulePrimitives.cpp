#include "Application.h"
#include "ModulePrimitives.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "assimp-vc142-mt.lib")

ModulePrimitives::ModulePrimitives(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModulePrimitives::~ModulePrimitives()
{}

bool ModulePrimitives::Init()
{
	bool ret = true;

	App->editor->LOG("Init. fbx loader");

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

update_status ModulePrimitives::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModulePrimitives::Update(float dt)
{
	if (App->editor->wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int i = 0; i < listFigures.Count(); i++)	
		Draw(listFigures.At(i)->data);

	return UPDATE_CONTINUE;
}

bool ModulePrimitives::CleanUp()
{
	bool ret = true;

	App->editor->LOG("Cleaning up fbx loader");
	aiDetachAllLogStreams();

	return ret;
}

void ModulePrimitives::CreateBuffer(Shape shape)
{
	GameObject figure;

	if(App->texture->listMaterials.At(0) == NULL)
		App->texture->LoadTextureBase();

	switch (shape)
	{
	case CUBE:
		figure = BufferQuad(figure);
		shapeCounter[0]++;
		figure.name = "CUBE_" + std::to_string(shapeCounter[0]);
		figure.shapename = "CUBE";
		App->editor->LOG("----- Creating primitive %s -----", figure.name.c_str());
		break;
	case PYRAMID:
		figure = BufferPyramid(figure);
		shapeCounter[1]++;
		figure.name = "PYRAMID_" + std::to_string(shapeCounter[1]);
		figure.shapename = "PYRAMID";
		App->editor->LOG("----- Creating primitive %s -----", figure.name.c_str());
		break;
	case FBX:
		shapeCounter[2]++;
		break;
	default:
		App->editor->LOG("Error: Shape not foud!");
		break;
	}

	figure.active = true;
	figure.currentMaterial = App->texture->listMaterials.At(0)->data;

	listFigures.Add(figure);
}

// Cube
GameObject ModulePrimitives::BufferQuad(GameObject figure)
{
	GLfloat vertices[] =
	{ 
		-0.5f, -0.5f, -0.5f,	// 8 of vertex coords
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,

		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f 
	};

	GLubyte indices[] = 
	{	
		0,2,1, 1,2,3,   // 36 of indices
		0,4,2, 4,6,2,
		0,1,4, 4,1,5,
		7,3,6, 6,3,2,
		7,6,5, 6,4,5,
		7,5,3, 5,1,3	
	};

	GLfloat uv[] =
	{
		0, 0,
		0, 1,
		1, 0,
		1, 1,

		1, 0,
		1, 1,
		0, 0,
		0, 1,
	};

	figure.num_vertices = 8;
	figure.num_indices = 36;
	figure.num_uvs = 8;
	figure.shape = CUBE;

	glGenBuffers(1, (GLuint*)&(figure.buff_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, figure.buff_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * figure.num_vertices * 3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*)&(figure.buff_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, figure.buff_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * figure.num_indices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*)&(figure.buff_uvs));
	glBindBuffer(GL_ARRAY_BUFFER, figure.buff_uvs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * figure.num_uvs * 2, uv, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	App->editor->LOG("----- Creating vertices (num: %d) -----", figure.num_vertices);
	App->editor->LOG("----- Creating buffer figure... -----");

	return figure;
}

//Pyramid
GameObject ModulePrimitives::BufferPyramid(GameObject figure)
{
	GLfloat vertices[] = 
	{	
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0, 0.5f, 0
	};

	GLubyte indices[] = 
	{
		0,2,1, 2,3,1,   // Base
		1,4,0,			// Side left
		0,4,2,			// Back
		2,4,3,			// Side right
		3,4,1			// Front 
	};	

	GLfloat uv[] =
	{
		0, 0,
		0, 1,
		1, 0,
		1, 1,

		1, 0,
		1, 1,
		0, 0,
		0, 1,
	};

	figure.num_vertices = 5;
	figure.num_indices = 18;
	figure.num_uvs = 8;
	figure.shape = PYRAMID;

	glGenBuffers(1, (GLuint*)&(figure.buff_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, figure.buff_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * figure.num_vertices * 3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*)&(figure.buff_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, figure.buff_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * figure.num_indices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*)&(figure.buff_uvs));
	glBindBuffer(GL_ARRAY_BUFFER, figure.buff_uvs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * figure.num_uvs * 2, uv, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	App->editor->LOG("----- Creating vertices (num: %d) -----", figure.num_vertices);
	App->editor->LOG("----- Creating buffer figure... -----");

	return figure;
}

void ModulePrimitives::Draw(GameObject figure)
{
	figure.rotation = Quat::FromEulerXYZ(figure.eulerRot.x * DEGTORAD, -figure.eulerRot.y * DEGTORAD, figure.eulerRot.z * DEGTORAD);
    figure.localTransform = figure.localTransform.FromTRS(figure.position, figure.rotation, figure.scale);
    figure.globalMatrix = figure.localTransform;
    glPushMatrix();
    glMultMatrixf(figure.globalMatrix.Transposed().ptr());

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, figure.buff_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, figure.buff_indices);

	glBindBuffer(GL_ARRAY_BUFFER, figure.buff_uvs);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, figure.currentMaterial.textureID);

	glDrawElements(GL_TRIANGLES, figure.num_indices, GL_UNSIGNED_BYTE, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();
}