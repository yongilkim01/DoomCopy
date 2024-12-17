#pragma once
#include "Classes/Components/SceneComponent.h"

/**
 *	������ Ŭ����
 */
class URenderer : public USceneComponent
{
	friend class ULevel;

public:
	/** UObject ��� �޼ҵ� */
	ENGINE_API URenderer();
	ENGINE_API ~URenderer();

	/** ��ü �� ���� ���� */
	URenderer(const URenderer& Other) = delete;
	URenderer(URenderer&& Other) noexcept = delete;
	URenderer& operator=(const URenderer& Other) = delete;
	URenderer& operator=(URenderer&& Other) noexcept = delete;

	/** UObject ��� �޼ҵ� */
	void SetOrder(int NewOrder) override;

protected:
	/** UObject ��� �޼ҵ� */
	ENGINE_API void BeginPlay() override;

private:
	/** URenderer �޼ҵ� */
	virtual void Render(float DeltaTime);

};