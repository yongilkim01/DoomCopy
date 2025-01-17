#pragma once
#include "Engine/Classes/Components/SceneComponent.h"
#include "Engine/Classes/Engine/PaperSprite.h"
#include "Engine/Classes/Engine/RenderAsset.h"

class UCameraComponent;

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

	//ENGINE_API virtual void UpdateCameraTransform(UCameraComponent* CameraComponent) override;

	/** 상속 메소드 */
	ENGINE_API virtual void BeginPlay() override;
	ENGINE_API virtual void Render(UCameraComponent* CameraComponent, float DeltaTime);
	ENGINE_API virtual void SetOrder(int NewOrder) override;

	/** 렌더 유닛 관련 메소드 */
	ENGINE_API URenderAsset& CreateRenderUnit();
	ENGINE_API URenderAsset& GetRenderUnit(UINT Index = 0);
	ENGINE_API void SetMesh(std::string_view Name, UINT Index = 0);
	ENGINE_API void SetMaterial(std::string_view Name, UINT Index = 0);

	/** 겟, 셋 메소드 */
	ENGINE_API std::vector<URenderAsset>& GetRenderUnitVector()
	{
		return RenderUnitVector;
	}
	ENGINE_API bool IsGravityEnabled()
	{
		return bEnableGravity;
	}
	ENGINE_API void SetEnableGravity(bool Value)
	{
		bEnableGravity = Value;
	}

protected:

private:
	std::vector<URenderAsset> RenderUnitVector;

	bool bEnableGravity = false;
};