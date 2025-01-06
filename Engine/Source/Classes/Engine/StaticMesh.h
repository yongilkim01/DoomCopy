#pragma once
#include "Classes/Engine/RenderAsset.h"
#include "Rendering/Buffer/IndexBuffer.h"
#include "Rendering/Buffer/VertexBuffer.h"

class AiMesh;
class UMesh;

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
	std::shared_ptr<UMesh> Mesh;
};

/**
 *	����
 */ 
class UStaticMesh : public URenderAsset
{
public:
	/** ������, �Ҹ��� */
	UStaticMesh();
	~UStaticMesh();

	/** ��ü �� ���� ���� */
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

	/** ��, �� �޼ҵ� */

protected:
	void CreateAsset(std::string_view FileName);

private:
	std::vector<FStaticMeshData> StaticMeshDataVector;
	std::map<int, std::shared_ptr<UTexture>> TextureMap;

	std::string DirectoryPath = "";
};

