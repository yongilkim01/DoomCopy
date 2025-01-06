#pragma once
#include "Classes/Engine/RenderAsset.h"
#include "Rendering/Buffer/IndexBuffer.h"
#include "Rendering/Buffer/VertexBuffer.h"

//struct EVertexData
//{
//	float4 POSITION;
//	float4 TEXCOORD;
//	float4 COLOR;
//};

class StaticMeshData
{
	//std::shared_ptr<FVertexBuffer> v;
	//std::shared_ptr<FIndexBuffer> is;

	//std::shared_ptr<UEngineMesh> Mesh;
	//std::shared_ptr<UEngineTexture> Texture;
	//std::shared_ptr<UMesh> Mesh;
};

/**
 *	설명
 */ 
class UStaticMesh : public URenderAsset
{
public:
	/** 생성자, 소멸자 */
	UStaticMesh();
	~UStaticMesh();

	/** 객체 값 복사 방지 */
	UStaticMesh(const UStaticMesh& Other) = delete;
	UStaticMesh(UStaticMesh&& Other) noexcept = delete;
	UStaticMesh& operator=(const UStaticMesh& Other) = delete;
	UStaticMesh& operator=(UStaticMesh&& Other) noexcept = delete;

	//bool LoadModel(std::string_view _objPath, std::string_view _mtlPath);
	//void ProcessNode(aiNode* node, const aiScene* scene);
	//void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	//std::vector<TEXTURE> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
	//ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* embeddedTexture);

	ENGINE_API static std::shared_ptr<UStaticMesh> Create(std::string_view Name)
	{
		return Create(Name, Name, Name);
	}
	ENGINE_API static std::shared_ptr<UStaticMesh> Create(std::string_view Name, std::string_view VertexBuffer, std::string_view IndexBuffer);

	void CreateAsset();

	/** 겟, 셋 메소드 */
	//std::shared_ptr<FVertexBuffer> GetVertexBuffer()
	//{
	//	return VertexBuffer;
	//}
	//std::shared_ptr<FIndexBuffer> GetIndexBuffer()
	//{
	//	return IndexBuffer;
	//}

protected:

private:
	std::vector<std::shared_ptr<FVertexBuffer>> VertexBuffer;

};

