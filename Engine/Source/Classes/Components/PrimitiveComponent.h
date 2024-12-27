#pragma once
#include "Classes/Components/SceneComponent.h"
#include "Classes/Engine/PaperSprite.h"
#include "Rendering/RenderUnit.h"

class ULevel;
class UCameraComponent;
class UPaperSprite;
class UTexture;

struct EngineVertex
{
	float4 POSITION;
	float4 TEXCOORD;
	float4 COLOR;
};
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

	/** �ʱ�ȭ �޼ҵ� */
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

	/** UPrimitiveComponent �޼ҵ� */
	ENGINE_API void SetSpriteData(size_t Index);
	ENGINE_API void SetSprite(std::string_view SpriteName);
	ENGINE_API void SetSprite(UPaperSprite* PaperSprite);

	/** UObject ��� �޼ҵ� */
	virtual void SetOrder(int NewOrder) override;

protected:
	/** UObject ��� �޼ҵ� */
	ENGINE_API void BeginPlay() override;

	/** URenderer �޼ҵ� */
	ENGINE_API virtual void Render(UCameraComponent* CameraComponent, float DeltaTime);

private:

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

	std::vector<URenderUnit> Units;

protected:
	UPaperSprite* Sprite = nullptr;

private:
	FPaperSpriteData SpriteData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> SpriteConstantBuffer = nullptr; // ��������Ʈ�� �������

};