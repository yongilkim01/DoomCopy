#include "pch.h"
#include "StaticMeshComponent.h"

#include "Classes/Camera/CameraComponent.h"
#include "Classes/Engine/Texture.h"

#include "Core/Misc/DirectoryHelper.h"
#include "Core/Misc/FileHelper.h"

#include "Rendering/TextureLoader.h"
#include "Rendering/AiMesh.h"
#include "Rendering/EngineVertex.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h> 
#include <assimp/scene.h>

HRESULT	CompileShaderFromFile(LPCWSTR pFileName, const D3D_SHADER_MACRO* pDefines, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3DBlob** ppBytecodeBlob)
{
	UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
	//UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;

#ifdef _DEBUG
	compileFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(pFileName, pDefines, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryPoint, pShaderModel, compileFlags, 0, ppBytecodeBlob, &pErrorBlob);
	if (FAILED(result))
	{
		if (pErrorBlob != nullptr)
		{
			OutputDebugStringA((LPCSTR)pErrorBlob->GetBufferPointer());
		}
	}

	if (pErrorBlob != nullptr)
	{
		pErrorBlob->Release();
	}

	return result;
}


UStaticMeshComponent::UStaticMeshComponent()
{
}

UStaticMeshComponent::~UStaticMeshComponent()
{
}

void UStaticMeshComponent::BeginPlay()
{
	UPrimitiveComponent::BeginPlay();
}

void UStaticMeshComponent::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	UPrimitiveComponent::Render(CameraComponent, DeltaTime);
}

void UStaticMeshComponent::InitShader()
{

}

void UStaticMeshComponent::InitObjFile(std::string_view DirectoryPath, std::string_view NewObjName, std::string_view NewObjPath, std::string_view NewMtlPath)
{
	ObjName = NewObjName;
	ObjPath = NewObjPath;
	MtlPath = NewMtlPath;

	LoadModel(ObjPath, MtlPath);

	for (int i = 0; i < MeshCount; i++)
	{
		CreateRenderUnit();
		SetMesh("E1M1" + std::to_string(i), i);
		SetMaterial("E1M1" + std::to_string(i), i);

		GetRenderUnit(i).SetTexture("ImageTexture", "NSBase.png");
	}
}

bool UStaticMeshComponent::LoadModel(std::string_view LoadObjPath, std::string_view LoadMtlPath)
{
	std::string fileName = LoadObjPath.data();
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr)
	{
		return false;
	}

	Directory = fileName.substr(0, fileName.find_last_of("/\\"));

	ProcessNode(pScene->mRootNode, pScene);

	return true;
}

void UStaticMeshComponent::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
		//Meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void UStaticMeshComponent::ProcessMesh(aiMesh* mesh, const aiScene* scene)
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

	FVertexBuffer::Create("E1M1" + std::to_string(MeshCount), Vertexs);
	FIndexBuffer::Create("E1M1" + std::to_string(MeshCount), Indexs);
	UMesh::Create("E1M1" + std::to_string(MeshCount));

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<TEXTURE> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		if (textures.size() > 0)
		{
			std::string TexturePath = Directory + "\\" + textures[0].path;

			TextureVector.push_back(UTexture::Load(TexturePath));

			std::string TextureType = textures[0].type;
		}

		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("E1M1" + std::to_string(MeshCount));
		Mat->SetVertexShader("MeshShader.fx");
		Mat->SetPixelShader("MeshShader.fx");
	}

	MeshCount++;

}

std::vector<TEXTURE> UStaticMeshComponent::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene)
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
				std::string filename = Directory + "\\" + std::string(str.C_Str());

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

ID3D11ShaderResourceView* UStaticMeshComponent::LoadEmbeddedTexture(const aiTexture* embeddedTexture)
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