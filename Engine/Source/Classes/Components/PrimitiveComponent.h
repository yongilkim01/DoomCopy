#pragma once
#include "Classes/Components/SceneComponent.h"
#include "Classes/Engine/PaperSprite.h"
#include "Rendering/RenderUnit.h"

class ULevel;
class UCameraComponent;
class UTexture;
class UStaticMesh;
class UEngineBlend;


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

	ENGINE_API URenderUnit& CreateRenderUnit();
	ENGINE_API URenderUnit& GetRenderUnit(UINT Index = 0);
	ENGINE_API void SetMesh(std::string_view Name, UINT Index = 0);
	ENGINE_API void SetMaterial(std::string_view Name, UINT Index = 0);

private:

public:

	std::vector<URenderUnit> Units;

protected:

private:

};