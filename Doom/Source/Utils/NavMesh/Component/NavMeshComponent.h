#pragma once
#include "Classes/Components/PrimitiveComponent.h"
#include "Classes/Engine/StaticMesh.h"
#include "Classes/Engine/Texture.h"
#include <Rendering/Buffer/EngineVertex.h>

class UNavMeshResource;

/**
 *	����
 */
class UNavMeshComponent : public UPrimitiveComponent
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UNavMeshComponent();
	ENGINE_API ~UNavMeshComponent();

	/** ��ü �� ���� ���� */
	UNavMeshComponent(const UNavMeshComponent& Other) = delete;
	UNavMeshComponent(UNavMeshComponent&& Other) noexcept = delete;
	UNavMeshComponent& operator=(const UNavMeshComponent& Other) = delete;
	UNavMeshComponent& operator=(UNavMeshComponent&& Other) noexcept = delete;

	std::vector<EngineVertex>& GetVertexVector();
	std::vector<unsigned int>& GetIndexVector();

protected:
	/** UObject ��� �޼ҵ� */
	ENGINE_API virtual void BeginPlay() override;

	/** UPrimitiveComponent ��� �޼ҵ� */
	ENGINE_API virtual void Render(UCameraComponent* CameraComponent, float DeltaTime) override;

private:
	std::shared_ptr<UNavMeshResource> NavMesh = nullptr;
};