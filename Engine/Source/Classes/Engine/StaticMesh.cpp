#include "pch.h"
#include "StaticMesh.h"

#include "Classes/Engine/Mesh.h"
#include "Classes/Engine/Texture.h"

#include "Core/Materials/Material.h"
#include "Core/Misc/DirectoryHelper.h"

#include "Rendering/TextureLoader.h"
#include "Rendering/AiMesh.h"
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

std::shared_ptr<UStaticMesh> UStaticMesh::Create(std::string_view FileName)
{
	std::string UpperName = ToUpperName(FileName);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드된 스테틱 메시입니다 " + UpperName);
		return nullptr;
	}

	std::shared_ptr<UStaticMesh> NewAsset = std::make_shared<UStaticMesh>();
	AddAsset<UStaticMesh>(NewAsset, FileName, "");
	NewAsset->CreateAsset(FileName);

	return NewAsset;
}


void UStaticMesh::CreateAsset(std::string_view FileName)
{
	FDirectoryHelper DirectoryHelper;

	if (false == DirectoryHelper.MoveParentToDirectory("Resources"))
	{
		MSGASSERT("리소스 폴더를 찾기에 실패했습니다");
		return;
	}

	DirectoryHelper.Append("Models\\E1M1");

	std::string Path = DirectoryHelper.GetPathToString();
	std::string ObjPath = Path + "\\" + FileName.data() + +".obj";
	std::string MtlPath = Path + "\\" + FileName.data() + +".mtl";

	LoadModel(ObjPath);

	//for (int i = 0; i < MeshCount; i++)
	//{
	//	CreateRenderUnit();
	//	SetMesh("E1M1" + std::to_string(i), i);
	//	SetMaterial("E1M1" + std::to_string(i), i);

	//	if (nullptr != TextureMap[i])
	//	{
	//		GetRenderUnit(i).SetTexture("ImageTexture", TextureMap[i]->GetName());
	//	}
	//}
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
	std::vector<TEXTURE> textures;


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

	FVertexBuffer::Create("E1M1" + std::to_string(StaticMeshDataVector.size()), Vertexs);
	FIndexBuffer::Create("E1M1" + std::to_string(StaticMeshDataVector.size()), Indexs);
	FStaticMeshData StaticMeshData;
	StaticMeshData.Mesh = UMesh::Create("E1M1" + std::to_string(StaticMeshDataVector.size()));

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<TEXTURE> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		if (textures.size() > 0)
		{
			std::string TexturePath = DirectoryPath + "\\" + textures[0].path;

			//TextureMap.push_back(UTexture::Load(TexturePath));
			TextureMap.insert(std::make_pair(StaticMeshDataVector.size(), UTexture::Load(TexturePath)));

			std::string TextureType = textures[0].type;
		}

		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("E1M1" + std::to_string(StaticMeshDataVector.size()));
		Mat->SetVertexShader("MeshShader.fx");
		Mat->SetPixelShader("MeshShader.fx");
	}

	StaticMeshDataVector.push_back(StaticMeshData);

}

std::vector<TEXTURE> UStaticMesh::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene)
{
	return std::vector<TEXTURE>();
}

ID3D11ShaderResourceView* UStaticMesh::LoadEmbeddedTexture(const aiTexture* embeddedTexture)
{
	return nullptr;
}
