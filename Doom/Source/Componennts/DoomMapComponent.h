#pragma once
#include <Engine/Classes/Components/PrimitiveComponent.h>
#include <Core/Math/EngineMath.h>

struct DoomMapVertex
{
	float4 POSITION;
	float4 COLOR;
};

/**
 *	설명
 */
class UDoomMapComponent : public UPrimitiveComponent
{
public:
	/** 생성자, 소멸자 */
	UDoomMapComponent();
	~UDoomMapComponent();

	/** 객체 값 복사 방지 */
	UDoomMapComponent(const UDoomMapComponent& Other) = delete;
	UDoomMapComponent(UDoomMapComponent&& Other) noexcept = delete;
	UDoomMapComponent& operator=(const UDoomMapComponent& Other) = delete;
	UDoomMapComponent& operator=(UDoomMapComponent&& Other) noexcept = delete;

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

protected:
	/** UObject 상속 메소드 */
	virtual void BeginPlay() override;

	/** URenderer 메소드 */
	virtual void Render(UCameraComponent* CameraComponent, float DeltaTime);

private:
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
};

