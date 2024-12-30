#pragma once
#include "Classes/Components/SceneComponent.h"
#include "Classes/Engine/PaperSprite.h"
#include "Rendering/RenderUnit.h"

class ULevel;
class UCameraComponent;
class UPaperSprite;
class UTexture;
class UStaticMesh;

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
	void InitRasterizer();

	void UpdateVertexBuffer();
	void UpdateIndexBuffer();
	void UpdateRasterizer();
	void UpdateRenderTargetView();

	/** 렌더링 파이프라인 상속 메소드 */
	virtual void InitShaderResourceView();
	virtual void UpdateShaderResourceView();

	virtual void InitVertexShader();
	virtual void UpdateVertexShader();

	virtual void InitPixelShader();
	virtual void UpdatePixelShader();




	/** UPrimitiveComponent 메소드 */
	ENGINE_API void SetMesh(std::string_view MeshName);


	/** UObject 상속 메소드 */
	ENGINE_API virtual void SetOrder(int NewOrder) override;

protected:
	/** UObject 상속 메소드 */
	ENGINE_API virtual void BeginPlay() override;

	/** URenderer 메소드 */
	ENGINE_API virtual void Render(UCameraComponent* CameraComponent, float DeltaTime);

private:

public:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> VSShaderCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> VSErrorCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader = nullptr;

	D3D11_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	D3D11_VIEWPORT ViewPortInfo;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerState = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> PSShaderCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> PSErrorCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState = nullptr;

	std::vector<URenderUnit> Units;

protected:

private:
	UStaticMesh* Mesh = nullptr;
};