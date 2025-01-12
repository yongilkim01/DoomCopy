#include "pch.h"
#include "Engine/Classes/Engine/StaticMesh.h"

#include "Engine/Classes/Engine/Mesh.h"
#include "Engine/Classes/Engine/Texture.h"

#include "Engine/Public//Materials/Material.h"
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

	if (false == DirectoryHelper.MoveParentToDirectory("Resources"))
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

	FVertexBuffer::Create(MeshName + std::to_string(StaticMeshDataVector.size()), Vertexs);
	FIndexBuffer::Create(MeshName + std::to_string(StaticMeshDataVector.size()), Indexs);
	FStaticMeshData StaticMeshData;
	StaticMeshData.Mesh = UMesh::Create(MeshName + std::to_string(StaticMeshDataVector.size()));

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<TEXTURE> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		if (textures.size() > 0)
		{
			std::string TexturePath = DirectoryPath + "\\" + textures[0].path;
			StaticMeshData.Texture = UTexture::Load(TexturePath);
		}

		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create(MeshName + std::to_string(StaticMeshDataVector.size()));
		Mat->SetVertexShader("MeshShader.fx");
		Mat->SetPixelShader("MeshShader.fx");
	}

	StaticMeshDataVector.push_back(StaticMeshData);

}

std::vector<TEXTURE> UStaticMesh::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene)
{
	std::vector<TEXTURE> textures;
	for (UINT i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (UINT j = 0; j < Textures.size(); j++)
		{
			if (std::strcmp(Textures[j].path.c_str(), str.C_Str()) == 0)
			{
				textures.push_back(Textures[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)	// If texture hasn't been loaded already, load it
		{
			HRESULT hr;
			TEXTURE texture;

			const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
			if (embeddedTexture != nullptr)
			{
				texture.texture = LoadEmbeddedTexture(embeddedTexture);
			}
			else
			{
				std::string filename = DirectoryPath + "\\" + std::string(str.C_Str());

				std::wstring filenamews = std::wstring(filename.begin(), filename.end());
				hr = CreateWICTextureFromFile(
					UEngineCore::GetDevice().GetDevice(), UEngineCore::GetDevice().GetDeviceContext(),
					filenamews.c_str(), nullptr, &texture.texture
				);

				if (FAILED(hr))
				{
					MSGASSERT("Texture couldn't be loaded");
				}
			}

			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			Textures.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}

	return textures;
}

ID3D11ShaderResourceView* UStaticMesh::LoadEmbeddedTexture(const aiTexture* embeddedTexture)
{
	HRESULT hr;
	ID3D11ShaderResourceView* texture = nullptr;

	if (embeddedTexture->mHeight != 0)
	{
		// Load an uncompressed ARGB8888 embedded texture
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = embeddedTexture->mWidth;
		desc.Height = embeddedTexture->mHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subresourceData;
		subresourceData.pSysMem = embeddedTexture->pcData;
		subresourceData.SysMemPitch = embeddedTexture->mWidth * 4;
		subresourceData.SysMemSlicePitch = embeddedTexture->mWidth * embeddedTexture->mHeight * 4;

		ID3D11Texture2D* texture2D = nullptr;
		hr = UEngineCore::GetDevice().GetDevice()->CreateTexture2D(&desc, &subresourceData, &texture2D);
		if (FAILED(hr))
		{
			MSGASSERT("CreateTexture2D failed!");
		}

		hr = UEngineCore::GetDevice().GetDevice()->CreateShaderResourceView(texture2D, nullptr, &texture);
		if (FAILED(hr))
		{
			MSGASSERT("CreateShaderResourceView failed");
		}

		return texture;
	}

	// mHeight is 0, so try to load a compressed texture of mWidth bytes
	const size_t size = embeddedTexture->mWidth;

	hr = CreateWICTextureFromMemory(
		UEngineCore::GetDevice().GetDevice(), UEngineCore::GetDevice().GetDeviceContext(),
		reinterpret_cast<const unsigned char*>(embeddedTexture->pcData), size,
		nullptr, &texture
	);

	if (FAILED(hr))
	{
		MSGASSERT("Texture couldn't be created from memory!");

	}

	return texture;
}
