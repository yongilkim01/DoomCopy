#include "pch.h"
#include "Engine/Classes/Engine/StaticMesh.h"

#include "Engine/Classes/Engine/Mesh.h"
#include "Engine/Classes/Engine/Texture.h"

#include "Engine/Public//Materials/Material.h"
#include "Core/Public/Misc/DirectoryHelper.h"

#include "Rendering/Buffer/EngineVertex.h"
#include "Rendering/Buffer/VertexBuffer.h"
#include "Rendering/Buffer/IndexBuffer.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h> 
#include <assimp/scene.h>

UStaticMesh::UStaticMesh()
{
}

UStaticMesh::~UStaticMesh()
{
}

std::shared_ptr<UStaticMesh> UStaticMesh::Create(std::string_view DirectoryName, std::string_view FileName)
{
	std::string UpperName = ToUpperName(FileName);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드된 스테틱 메시입니다 " + UpperName);
		return nullptr;
	}

	std::shared_ptr<UStaticMesh> NewAsset = std::make_shared<UStaticMesh>();
	AddAsset<UStaticMesh>(NewAsset, FileName, "");
	NewAsset->CreateAsset(DirectoryName, FileName);

	return NewAsset;
}


std::string UStaticMesh::GetMeshNameByIndex(int Index)
{
	return StaticMeshDataVector[Index].Mesh->GetName();
}

std::string UStaticMesh::GetTextureNameByIndex(int Index)
{
	return StaticMeshDataVector[Index].Texture->GetName();
}

std::shared_ptr<UTexture> UStaticMesh::GetTextureByIndex(int Index)
{
	return StaticMeshDataVector[Index].Texture;
}

std::shared_ptr<UMesh> UStaticMesh::GetMeshByIndex(int Index)
{
	return StaticMeshDataVector[Index].Mesh;
}

void UStaticMesh::CreateAsset(std::string_view DirectoryName, std::string_view FileName)
{
	FDirectoryHelper DirectoryHelper;

	if (false == DirectoryHelper.MoveParentToDirectory("Resources", "Doom"))
	{
		MSGASSERT("리소스 폴더를 찾기에 실패했습니다");
		return;
	}

	DirectoryHelper.Append("Models\\");
	DirectoryHelper.Append(DirectoryName);

	std::string Path = DirectoryHelper.GetPathToString();
	std::string ObjPath = Path + "\\" + FileName.data() + +".obj";
	std::string MtlPath = Path + "\\" + FileName.data() + +".mtl";

	MeshName = FileName;

	LoadModel(ObjPath);
}


bool UStaticMesh::LoadModel(std::string_view LoadObjPath)
{
	std::string FileName = LoadObjPath.data();
	Assimp::Importer AssimpImporter;

	const aiScene* pScene = AssimpImporter.ReadFile(FileName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr)
	{
		return false;
	}

	DirectoryPath = FileName.substr(0, FileName.find_last_of("/\\"));

	ProcessNode(pScene->mRootNode, pScene);

	return true;
}

void UStaticMesh::ProcessNode(aiNode* AssimpNode, const aiScene* AssimpScene)
{
	for (UINT i = 0; i < AssimpNode->mNumMeshes; i++)
	{
		aiMesh* AssimpMesh = AssimpScene->mMeshes[AssimpNode->mMeshes[i]];

		ProcessMesh(AssimpMesh, AssimpScene);
	}

	for (UINT i = 0; i < AssimpNode->mNumChildren; i++)
	{
		ProcessNode(AssimpNode->mChildren[i], AssimpScene);
	}
}

void UStaticMesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<EngineVertex> Vertexs;
	std::vector<unsigned int> Indexs;


	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		EngineVertex Vertex = EngineVertex{ FVector(-0.5f, 0.5f, 0.0f), {0.0f , 0.0f }, {1.0f, 1.0f, 1.0f, 1.0f} };

		Vertex.POSITION.X = mesh->mVertices[i].x;
		Vertex.POSITION.Y = mesh->mVertices[i].y;
		Vertex.POSITION.Z = mesh->mVertices[i].z;
		Vertex.POSITION.W = 1.0f;

		if (mesh->mTextureCoords[0])
		{
			Vertex.TEXCOORD.X = (float)mesh->mTextureCoords[0][i].x;
			Vertex.TEXCOORD.Y = (float)mesh->mTextureCoords[0][i].y;
		}

		Vertexs.push_back(Vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			Indexs.push_back(face.mIndices[j]);
		}
	}

	FVertexBuffer::Create(MeshName + std::to_string(StaticMeshDataVector.size()), Vertexs);
	FIndexBuffer::Create(MeshName + std::to_string(StaticMeshDataVector.size()), Indexs);
	FStaticMeshData StaticMeshData;
	StaticMeshData.Mesh = UMesh::Create(MeshName + std::to_string(StaticMeshDataVector.size()));

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::string TextureName = LoadTextureName(material, aiTextureType_DIFFUSE);

		if ("" != TextureName)
		{
			std::string TexturePath = DirectoryPath + "\\" + TextureName;
			StaticMeshData.Texture = UTexture::Load(TexturePath);
		}

		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create(MeshName + std::to_string(StaticMeshDataVector.size()));

		Mat->SetVertexShader("MeshShader.fx");
		Mat->SetPixelShader("MeshShader.fx");
	}

	StaticMeshDataVector.push_back(StaticMeshData);

}

std::string UStaticMesh::LoadTextureName(aiMaterial* mat, aiTextureType type)
{
	std::string result = "";

	for (UINT i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		result = str.C_Str();
	}

	return result;
}