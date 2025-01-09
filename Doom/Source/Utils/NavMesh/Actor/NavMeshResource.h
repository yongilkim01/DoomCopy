#pragma once
#include <Classes/Engine/RenderAsset.h>
#include <Rendering/Buffer/IndexBuffer.h>
#include <Rendering/Buffer/VertexBuffer.h>

class UMesh;
class UTexture;

struct FNavMeshResourceData
{
public:
	std::shared_ptr<UMesh> Mesh = nullptr;
	std::shared_ptr<UTexture> Texture = nullptr;
};

/**
 *	����
 */
class UNavMeshResource : public URenderAsset
{
public:
	/** ������, �Ҹ��� */
	UNavMeshResource();
	~UNavMeshResource();

	/** ��ü �� ���� ���� */
	UNavMeshResource(const UNavMeshResource& Other) = delete;
	UNavMeshResource(UNavMeshResource&& Other) noexcept = delete;
	UNavMeshResource& operator=(const UNavMeshResource& Other) = delete;
	UNavMeshResource& operator=(UNavMeshResource&& Other) noexcept = delete;

	ENGINE_API static std::shared_ptr<UNavMeshResource> Create(std::string_view ModelName);

	bool LoadModel(std::string_view ModelName);

	/** ��, �� �޼ҵ� */
	size_t GetNavMeshResourceCount()
	{
		return NavMeshResourceDataVector.size();
	}
	std::string GetMeshNameByIndex(int Index);
	std::string GetTextureNameByIndex(int Index);
	std::shared_ptr<UTexture> GetTextureByIndex(int Index);
	std::shared_ptr<UMesh> GetMeshByIndex(int Index);

protected:
	void CreateAsset(std::string_view ModelName);

private:
	std::vector<FNavMeshResourceData> NavMeshResourceDataVector;
	std::string MeshName = "";
	std::string DirectoryPath = "";
};

