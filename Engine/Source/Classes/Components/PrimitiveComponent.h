#pragma once
#include "Classes/Components/SceneComponent.h"
#include "Classes/Engine/PaperSprite.h"
#include "Rendering/RenderUnit.h"

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

	ENGINE_API virtual void RenderTransformUpdate(UCameraComponent* CameraComponent);

	/** 상속 메소드 */
	ENGINE_API virtual void BeginPlay() override;
	ENGINE_API virtual void Render(UCameraComponent* CameraComponent, float DeltaTime);
	ENGINE_API virtual void SetOrder(int NewOrder) override;

	/** 렌더 유닛 관련 메소드 */
	ENGINE_API URenderUnit& CreateRenderUnit();
	ENGINE_API URenderUnit& GetRenderUnit(UINT Index = 0);
	ENGINE_API void SetMesh(std::string_view Name, UINT Index = 0);
	ENGINE_API void SetMaterial(std::string_view Name, UINT Index = 0);

	/** 겟, 셋 메소드 */
	ENGINE_API std::vector<URenderUnit>& GetRenderUnitVector()
	{
		return RenderUnitVector;
	}

protected:

private:
	std::vector<URenderUnit> RenderUnitVector;

};