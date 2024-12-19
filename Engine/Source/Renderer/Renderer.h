#pragma once
#include "Classes/Components/SceneComponent.h"

struct EngineVertex
{
public:
	FVector Position;
	FVector Color;
};

/**
 *	렌더러 클래스
 */
class URenderer : public USceneComponent
{
	friend class ULevel;

public:
	/** UObject 상속 메소드 */
	ENGINE_API URenderer();
	ENGINE_API ~URenderer();

	/** 객체 값 복사 방지 */
	URenderer(const URenderer& Other) = delete;
	URenderer(URenderer&& Other) noexcept = delete;
	URenderer& operator=(const URenderer& Other) = delete;
	URenderer& operator=(URenderer&& Other) noexcept = delete;

	/** UObject 상속 메소드 */
	void SetOrder(int NewOrder) override;

protected:
	/** UObject 상속 메소드 */
	ENGINE_API void BeginPlay() override;

private:
	/** URenderer 메소드 */
	virtual void Render(float DeltaTime);

public:
	ID3D11Buffer* VertexBuffer = nullptr;
	void InputAssembler1Init();
	void InputAssembler1Setting();
};