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

	/** 렌더링 파이프라인 메소드 */
	void InitVertexLayout();
	virtual void InitRasterizer();

	void UpdateVertexBuffer();
	void UpdateVertexShader();
	void UpdateIndexBuffer();
	void UpdateRasterizer();	
	void UpdatePixelShader();
	void UpdateRenderTargetView();

	/** 렌더링 파이프라인 상속 메소드 */
	virtual void InitShaderResourceView();
	virtual void UpdateShaderResourceView();

	virtual void InitVertexShader();
	virtual void InitPixelShader();

	/** UObject 상속 메소드 */
	ENGINE_API void SetOrder(int NewOrder);

protected:
	/** UObject 상속 메소드 */
	ENGINE_API virtual void BeginPlay() override;

	/** URenderer 메소드 */
	ENGINE_API virtual void Render(UCameraComponent* CameraComponent, float DeltaTime);

	URenderUnit& CreateRenderUnit();

private:

public:

	std::vector<URenderUnit> Units;

protected:

private:

};