#include "Application.h"
#include "ModuleTextures.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "devIL.lib")  
#pragma comment (lib, "ILU.lib") 
#pragma comment (lib, "ILUT.lib")

ModuleTextures::ModuleTextures(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleTextures::~ModuleTextures()
{}

bool ModuleTextures::Init()
{
	bool ret = true;

	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
		iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION ||
		ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION) {
		printf("DevIL version is different...exiting!\n");
	}

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return ret;
}

update_status ModuleTextures::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleTextures::Update(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleTextures::CleanUp()
{
	bool ret = true;

	return ret;
}

void ModuleTextures::LoadTextureBase()
{
	Material material;

	GLubyte checkerImage[64][64][4];
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &material.textureID);
	glBindTexture(GL_TEXTURE_2D, material.textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);

	material.texWidth = 64;
	material.texHeight = 64;
	material.checker = true;
	material.textureID = GLint(checkerImage);

	listMaterials.Add(material);
}

void ModuleTextures::LoadTextures(const char* file_path)
{
	Material material;

	//Image
	ILuint imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	//Load
	if (ilLoadImage(file_path))
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &material.textureID);
		glBindTexture(GL_TEXTURE_2D, material.textureID);

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
		
		material.texWidth = ilGetInteger(IL_IMAGE_WIDTH);
		material.texHeight = ilGetInteger(IL_IMAGE_HEIGHT);
		material.checker = false;

		material.textureID = ilutGLBindTexImage();

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else App->editor->LOG("Error: Could not open the image!");

	ilDeleteImages(1, &imageID);

	listMaterials.Add(material);
}