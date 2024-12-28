#pragma once
#include <Classes/Components/PrimitiveComponent.h>
#include <Core/Math/EngineMath.h>

struct LineVertex
{
	float4 POSITION;
	float4 COLOR;
};

/**
 *	����
 */
class ULineComponent : public UPrimitiveComponent
{
public:
	/** ������, �Ҹ��� */
	ULineComponent();
	~ULineComponent();

	/** ��ü �� ���� ���� */
	ULineComponent(const ULineComponent& Other) = delete;
	ULineComponent(ULineComponent&& Other) noexcept = delete;
	ULineComponent& operator=(const ULineComponent& Other) = delete;
	ULineComponent& operator=(ULineComponent&& Other) noexcept = delete;

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
	/** UObject ��� �޼ҵ� */
	virtual void BeginPlay() override;

	/** URenderer �޼ҵ� */
	virtual void Render(UCameraComponent* CameraComponent, float DeltaTime);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> VSShaderCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> VSErrorCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader = nullptr;

	D3D11_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer = nullptr;

	D3D11_VIEWPORT ViewPortInfo;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerState = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> PSShaderCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> PSErrorCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState = nullptr;
};

