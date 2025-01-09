#pragma once
#include "Classes/Components/PrimitiveComponent.h"
#include "Classes/Engine/StaticMesh.h"
#include "Classes/Engine/Texture.h"
#include <Rendering/Buffer/EngineVertex.h>

class UNavMeshResource;

/**
 *	설명
 */
class UNavMeshComponent : public UPrimitiveComponent
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UNavMeshComponent();
	ENGINE_API ~UNavMeshComponent();

	/** 객체 값 복사 방지 */
	UNavMeshComponent(const UNavMeshComponent& Other) = delete;
	UNavMeshComponent(UNavMeshComponent&& Other) noexcept = delete;
	UNavMeshComponent& operator=(const UNavMeshComponent& Other) = delete;
	UNavMeshComponent& operator=(UNavMeshComponent&& Other) noexcept = delete;

	std::vector<EngineVertex>& GetVertexVector();
	std::vector<unsigned int>& GetIndexVector();

protected:
	/** UObject 상속 메소드 */
	ENGINE_API virtual void BeginPlay() override;

	/** UPrimitiveComponent 상속 메소드 */
	ENGINE_API virtual void Render(UCameraComponent* CameraComponent, float DeltaTime) override;

private:
	std::shared_ptr<UNavMeshResource> NavMesh = nullptr;
};