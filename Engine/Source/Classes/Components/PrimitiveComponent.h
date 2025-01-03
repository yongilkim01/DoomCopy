#pragma once
#include "Classes/Components/SceneComponent.h"
#include "Classes/Engine/PaperSprite.h"
#include "Rendering/RenderUnit.h"

class ULevel;
class UCameraComponent;
class UTexture;
class UStaticMesh;
class UEngineBlend;

struct FUVValue
{
	float4 PlusUVValue;
};

/**
 *	������ Ŭ����
 */
class UPrimitiveComponent : public USceneComponent
{
	friend UCameraComponent;

public:
	/** UObject ��� �޼ҵ� */
	ENGINE_API UPrimitiveComponent();
	ENGINE_API ~UPrimitiveComponent();

	/** ��ü �� ���� ���� */
	UPrimitiveComponent(const UPrimitiveComponent& Other) = delete;
	UPrimitiveComponent(UPrimitiveComponent&& Other) noexcept = delete;
	UPrimitiveComponent& operator=(const UPrimitiveComponent& Other) = delete;
	UPrimitiveComponent& operator=(UPrimitiveComponent&& Other) noexcept = delete;

	/** UObject ��� �޼ҵ� */
	ENGINE_API void SetOrder(int NewOrder);

protected:
	/** UObject ��� �޼ҵ� */
	ENGINE_API virtual void BeginPlay() override;

	/** URenderer �޼ҵ� */
	ENGINE_API virtual void Render(UCameraComponent* CameraComponent, float DeltaTime);

	URenderUnit& CreateRenderUnit();
	URenderUnit& GetRenderUnit(UINT Index);
	void SetMesh(std::string_view Name, UINT Index = 0);
	void SetMaterial(std::string_view Name, UINT Index = 0);

private:

public:

	std::vector<URenderUnit> Units;

protected:

private:

};