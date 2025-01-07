#pragma once
#include "Classes/Engine/RenderAsset.h"
#include "Rendering/Buffer/IndexBuffer.h"
#include "Rendering/Buffer/VertexBuffer.h"

class AiMesh;
class UMesh;
class UTexture;

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
struct aiTexture;
struct VERTEX;
struct TEXTURE;

enum aiTextureType;

struct FStaticMeshData
{
public:
	std::shared_ptr<UMesh> Mesh = nullptr;
	std::shared_ptr<UTexture> Texture = nullptr;
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

	ENGINE_API static std::shared_ptr<UStaticMesh> Create(std::string_view FileName);

	bool LoadModel(std::string_view LoadObjPath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<TEXTURE> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
	ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* embeddedTexture);

	/** 겟, 셋 메소드 */
	size_t GetStaticMeshCount()
	{
		return StaticMeshDataVector.size();
	}
	std::string GetMeshNameByIndex(int Index);
	std::string GetTextureNameByIndex(int Index);
	std::shared_ptr<UTexture> GetTextureByIndex(int Index);
	std::shared_ptr<UMesh> GetMeshByIndex(int Index);

protected:
	void CreateAsset(std::string_view FileName);

private:
	std::vector<FStaticMeshData> StaticMeshDataVector;
	std::map<int, std::shared_ptr<UTexture>> TextureMap;
	std::vector<TEXTURE> Textures;
	std::string MeshName = "";
	std::string DirectoryPath = "";
};

