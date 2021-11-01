#include "Application.h"
#include "ModuleFBX.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "assimp-vc142-mt.lib")

ModuleFBX::ModuleFBX(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleFBX::~ModuleFBX()
{
}


void ModuleFBX::LoadFbx(const char* file_path)
{
    const aiScene* pscene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
    if (pscene != nullptr)
    {
        GameObject figure;

        // Initialize the meshes in the scene one by one
        for (unsigned int i = 0; i < pscene->mNumMeshes; i++) {
            const aiMesh* paiMesh = pscene->mMeshes[i];
            figure.name = pscene->mMeshes[i]->mName.C_Str();
            App->editor->LOG("----- Creating primitive %s -----", figure.name.c_str());
            InitMesh(figure, paiMesh);
        }
    }
    else App->editor->LOG("Error loading scene %s", file_path);
}

void ModuleFBX::InitMesh(GameObject figure, const aiMesh* paiMesh)
{
    std::vector<float3> Vertices;
    std::vector<unsigned int> Indices;

    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
        const aiVector3D* pPos = &(paiMesh->mVertices[i]);

        Vertices.push_back(float3(pPos->x, pPos->y, pPos->z));
    }

    for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    figure.num_vertices = Vertices.size();
    figure.num_indices = Indices.size();
    figure.shape = FBX;
    figure.active = true;

    glGenBuffers(1, &figure.buff_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, figure.buff_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &figure.buff_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, figure.buff_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    App->editor->LOG("----- Creating vertices (num: %d) -----", figure.num_vertices);
    App->editor->LOG("----- Creating buffer figure... -----");

    App->primitive->listFigures.Add(figure);
}
   
