#pragma once
#include "Classes/Components/PrimitiveComponent.h"
#include "Classes/Engine/StaticMesh.h"
#include "Classes/Engine/Texture.h"

class UStaticMesh;

/**
 *	����
 */
class UTestComponent : public UPrimitiveComponent
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UTestComponent();
	ENGINE_API ~UTestComponent();

	/** ��ü �� ���� ���� */
	UTestComponent(const UTestComponent& Other) = delete;
	UTestComponent(UTestComponent&& Other) noexcept = delete;
	UTestComponent& operator=(const UTestComponent& Other) = delete;
	UTestComponent& operator=(UTestComponent&& Other) noexcept = delete;

protected:
	/** UObject ��� �޼ҵ� */
	ENGINE_API virtual void BeginPlay() override;

	/** UPrimitiveComponent ��� �޼ҵ� */
	ENGINE_API virtual void Render(UCameraComponent* CameraComponent, float DeltaTime) override;

private:

};