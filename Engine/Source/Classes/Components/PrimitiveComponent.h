#pragma once
#include "Classes/Components/SceneComponent.h"
#include "Classes/Engine/PaperSprite.h"

class ULevel;
class UCameraComponent;
class UTexture;

struct EngineVertex
{
	float4 POSITION;
	float4 TEXCOORD;
	float4 COLOR;
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

	/** 초기화 메소드 */
	void InitVertexBuffer();
	void InitVertexLayout();
	void UpdateVertexBuffer();

	void InitVertexShader();
	void UpdateVertexShader();

	void InitIndexBuffer();
	void UpdateIndexBuffer();

	void InitRasterizer();
	void UpdateRasterizer();

	void InitPixelShader();
	void UpdatePixelShader();

	void InitShaderResourceView();
	void UpdateShaderResourceView();

	void UpdateRenderTargetView();

	/** URenderer 메소드 */
	ENGINE_API void SetSpriteData(size_t Index);
	void SetTexture(std::string_view TextureName);

	/** UObject 상속 메소드 */
	virtual void SetOrder(int NewOrder) override;

protected:
	/** UObject 상속 메소드 */
	ENGINE_API void BeginPlay() override;

private:
	/** URenderer 메소드 */
	virtual void Render(UCameraComponent* CameraComponent, float DeltaTime);

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> VSShaderCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> VSErrorCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader = nullptr;

	D3D11_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer = nullptr;

	D3D11_VIEWPORT ViewPortInfo;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerState = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> PSShaderCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> PSErrorCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState = nullptr;

private:
	FPaperSpriteData SpriteData;
	std::shared_ptr<UPaperSprite> Sprite = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> SpriteConstantBuffer = nullptr; // 스프라이트용 상수버퍼

};