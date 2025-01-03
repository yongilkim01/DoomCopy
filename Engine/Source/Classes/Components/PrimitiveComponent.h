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
 *	렌더러 클래스
 */
class UPrimitiveComponent : public USceneComponent
{
	friend UCameraComponent;

public:
	/** UObject 상속 메소드 */
	ENGINE_API UPrimitiveComponent();
	ENGINE_API ~UPrimitiveComponent();

	/** 객체 값 복사 방지 */
	UPrimitiveComponent(const UPrimitiveComponent& Other) = delete;
	UPrimitiveComponent(UPrimitiveComponent&& Other) noexcept = delete;
	UPrimitiveComponent& operator=(const UPrimitiveComponent& Other) = delete;
	UPrimitiveComponent& operator=(UPrimitiveComponent&& Other) noexcept = delete;

	/** UObject 상속 메소드 */
	ENGINE_API void SetOrder(int NewOrder);

protected:
	/** UObject 상속 메소드 */
	ENGINE_API virtual void BeginPlay() override;

	/** URenderer 메소드 */
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