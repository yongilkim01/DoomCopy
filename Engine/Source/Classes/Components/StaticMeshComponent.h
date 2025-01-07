#pragma once
#include "Classes/Components/PrimitiveComponent.h"
#include "Classes/Engine/StaticMesh.h"
#include "Classes/Engine/Texture.h"

class UStaticMesh;

/**
 *	����
 */
class UStaticMeshComponent : public UPrimitiveComponent
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UStaticMeshComponent();
	ENGINE_API ~UStaticMeshComponent();

	/** ��ü �� ���� ���� */
	UStaticMeshComponent(const UStaticMeshComponent& Other) = delete;
	UStaticMeshComponent(UStaticMeshComponent&& Other) noexcept = delete;
	UStaticMeshComponent& operator=(const UStaticMeshComponent& Other) = delete;
	UStaticMeshComponent& operator=(UStaticMeshComponent&& Other) noexcept = delete;

	ENGINE_API void SetModel(std::string_view ModelName);
	//ENGINE_API void SetModelExceptIndex(std::string_view ModelName, std::vector)

protected:
	/** UObject ��� �޼ҵ� */
	ENGINE_API virtual void BeginPlay() override;

	/** UPrimitiveComponent ��� �޼ҵ� */
	ENGINE_API virtual void Render(UCameraComponent* CameraComponent, float DeltaTime) override;

private:
	std::shared_ptr<UStaticMesh> StaticMesh = nullptr;

	int MeshCount = 0;
};