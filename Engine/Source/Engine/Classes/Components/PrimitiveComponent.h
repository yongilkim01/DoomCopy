#pragma once
#include "Engine/Classes/Components/SceneComponent.h"
#include "Engine/Classes/Engine/PaperSprite.h"
#include "Engine/Classes/Engine/RenderAsset.h"

class UCameraComponent;

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

	//ENGINE_API virtual void UpdateCameraTransform(UCameraComponent* CameraComponent) override;

	/** ��� �޼ҵ� */
	ENGINE_API virtual void BeginPlay() override;
	ENGINE_API virtual void Render(UCameraComponent* CameraComponent, float DeltaTime);
	ENGINE_API virtual void SetOrder(int NewOrder) override;

	/** ���� ���� �޼ҵ� */
	/**
	 *	������ �̵�, �浹 ó��, Ʈ������ ������Ʈ�� ����ϴ� �޼ҵ�
	 * 
	 *	@param Delta - ������Ʈ�� �̵��� ���� ���ͷ� �̵� ����� �Ÿ��� ��Ÿ����.
	 *  @param NewRotation - �̵� �� ������Ʈ�� ���ο� ȸ���� ��Ÿ����.
	 *  @param bSweep - Sweep�� ������ �� ���θ� �����Ѵ�, true�� ��� �浹 �˻縦 Ȱ��ȭ�Ͽ� �̵� ��ο��� �浹�� Ȯ���Ѵ�.
	 */
	ENGINE_API bool MoveComponent(const FVector& Delta, const FVector& NewRotation, bool bSweep/*, FHitResult* OutHit, EMoveComponentFlags MoveFlags, ETeleportType Teleport*/);

	/** ���� ���� ���� �޼ҵ� */
	ENGINE_API URenderAsset& CreateRenderUnit();
	ENGINE_API URenderAsset& GetRenderUnit(UINT Index = 0);
	ENGINE_API void SetMesh(std::string_view Name, UINT Index = 0);
	ENGINE_API void SetMaterial(std::string_view Name, UINT Index = 0);

	/** ��, �� �޼ҵ� */
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