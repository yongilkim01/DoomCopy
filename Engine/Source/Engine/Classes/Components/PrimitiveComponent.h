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

	/** 물리 관련 메소드 */
	/**
	 *	물리적 이동, 충돌 처리, 트랜스폼 업데이트를 담당하는 메소드
	 * 
	 *	@param Delta - 컴포넌트가 이동할 변위 벡터로 이동 방향과 거리를 나타낸다.
	 *  @param NewRotation - 이동 후 컴포넌트의 새로운 회전을 나타낸다.
	 *  @param bSweep - Sweep을 수행할 지 여부를 결정한다, true일 경우 충돌 검사를 활성화하여 이동 경로에서 충돌을 확인한다.
	 */
	ENGINE_API bool MoveComponent(const FVector& Delta, const FVector& NewRotation, bool bSweep/*, FHitResult* OutHit, EMoveComponentFlags MoveFlags, ETeleportType Teleport*/);

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