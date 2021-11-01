#include "Application.h"
#include "ModuleFBX.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "assimp-vc142-mt.lib")


ModuleFBX::ModuleFBX(Application* app, bool start_enabled) : Module (app, start_enabled)
{
}


ModuleFBX::~ModuleFBX()
{
}

bool ModuleFBX::Init()
{
    bool ret = true;

    struct aiLogStream stream;
    stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
    aiAttachLogStream(&stream);

    return ret;
}

bool ModuleFBX::CleanUp()
{
    bool ret = true;

    App->editor->LOG("Cleaning up fbx loader");
    aiDetachAllLogStreams();

    return ret;
}


void ModuleFBX::LoadFbx(const char* file_path)
{
    const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
    if (scene != nullptr)
    {
        if (scene->HasMeshes())
        {
            GameObject figure;
            for (int i = 0; i < scene->mNumMeshes; i++) {
                figure.name = scene->mMeshes[i]->mName.C_Str();
                const aiMesh* aiFbx = scene->mMeshes[i];

                InitFbx(aiFbx, figure);
            }
        }
    }
    else App->editor->LOG("Error loading scene %s", file_path);

    aiReleaseImport(scene);
}


void ModuleFBX::InitFbx(const aiMesh* aiMesh, GameObject figure)
{
    std::vector<float3> Vertices;
    std::vector<int> Indices;

    //const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < aiMesh->mNumVertices; i++) {
        //const aiVector3D* pTexCoord = aiMesh->HasTextureCoords(0) ? &(aiMesh->mTextureCoords[0][i]) : &Zero3D;

        float3 vertx(aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z);
        Vertices.push_back(vertx);
    }

    for (int i = 0; i < aiMesh->mNumFaces; i++) {
        const aiFace& Face = aiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    figure.num_vertices = Vertices.size();
    figure.num_indices = Indices.size();
    figure.shape = FBX;
    //figure.num_uvs = aiMesh->mNumVertices;

    glGenBuffers(1, &(figure.buff_vertices));
    glBindBuffer(GL_ARRAY_BUFFER, figure.buff_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * figure.num_vertices, &Vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &(figure.buff_indices));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, figure.buff_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * figure.num_indices, &Indices, GL_STATIC_DRAW);

    //glGenBuffers(1, (GLuint*)&(figure.buff_uvs));
    //glBindBuffer(GL_ARRAY_BUFFER, figure.buff_uvs);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * figure.num_uvs * 2, &Vertices[0].mTexture, GL_STATIC_DRAW);

    App->primitive->listFigures.Add(figure);
}
   
