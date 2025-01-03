#include "pch.h"
#include "StaticMeshComponent.h"

#include "Classes/Camera/CameraComponent.h"

#include "Core/Misc/DirectoryHelper.h"
#include "Core/Misc/FileHelper.h"

#include "Rendering/TextureLoader.h"
#include "Rendering/AiMesh.h"

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
	for (AiMesh& mesh : Meshes)
	{
		mesh.Close();
	}

	for (TEXTURE& tex : Textures)
	{
		tex.Release();
	}
}

void UStaticMeshComponent::BeginPlay()
{
	USceneComponent::BeginPlay();
	SetOrder(0);

	LoadModel(ObjPath, MtlPath);

	size_t MeshSize = Meshes.size();
	size_t IndexSize = Textures.size();
}

void UStaticMeshComponent::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	FTransform& CameraTransform = CameraComponent->GetComponentTransformRef();
	FTransform& RendererTransform = GetComponentTransformRef();

	RendererTransform.View = CameraTransform.View;
	RendererTransform.Projection = CameraTransform.Projection;
	RendererTransform.WVP = RendererTransform.World * CameraTransform.View * CameraTransform.Projection;

}

void UStaticMeshComponent::InitShader()
{
	//// 현재 디렉토리 헬퍼 객체 생성
	//FDirectoryHelper CurDir;
	//// 엔진 쉐이더 디렉토리로 이동
	//CurDir.MoveEngineShaderDirectory();
	//// 쉐이더 파일을 가져옴
	//FFileHelper VSFile = CurDir.GetFile("MeshShader.fx");
	//FFileHelper PSFile = CurDir.GetFile("MeshShader.fx");

	//std::wstring vsPath = UEngineString::AnsiToUnicode(VSFile.GetPathToString());
	//std::wstring psPath = UEngineString::AnsiToUnicode(PSFile.GetPathToString());

	//HRESULT Result = CompileShaderFromFile(vsPath.c_str(), 0, "VS", "vs_5_0", &VSShaderCodeBlob);

	//if (S_OK != Result)
	//{
	//	MSGASSERT("Failed to compile vertex shader from file");
	//}


	//Result = CompileShaderFromFile(psPath.c_str(), 0, "PS", "ps_5_0", &PSShaderCodeBlob);
	//
	//if (S_OK != Result)
	//{
	//	MSGASSERT("Failed to compile pixel shader from file");
	//}

	//UEngineCore::GetDevice().GetDevice()->CreateVertexShader(VSShaderCodeBlob->GetBufferPointer(), VSShaderCodeBlob->GetBufferSize(), nullptr, &VertexShader);
	//UEngineCore::GetDevice().GetDevice()->CreatePixelShader(PSShaderCodeBlob->GetBufferPointer(), PSShaderCodeBlob->GetBufferSize(), nullptr, &PixelShader);

	//D3D11_INPUT_ELEMENT_DESC ied[] =
	//{
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//};

	//UEngineCore::GetDevice().GetDevice()->CreateInputLayout(ied, 2, VSShaderCodeBlob->GetBufferPointer(), VSShaderCodeBlob->GetBufferSize(), &InputLayout);
}

void UStaticMeshComponent::Init(std::string_view NewObjPath, std::string_view NewMtlPath)
{
	ObjPath = NewObjPath;
	MtlPath = NewMtlPath;
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
		Meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

AiMesh UStaticMeshComponent::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<VERTEX> vertices;
	std::vector<UINT> indices;
	std::vector<TEXTURE> textures;

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		VERTEX vertex;

		vertex.X = mesh->mVertices[i].x;
		vertex.Y = mesh->mVertices[i].y;
		vertex.Z = mesh->mVertices[i].z;
		vertex.W = 1.0f;

		if (mesh->mTextureCoords[0]) {
			vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<TEXTURE> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}

	return AiMesh(UEngineCore::GetDevice().GetDevice(), vertices, indices, textures);
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