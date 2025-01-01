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

	/** ������ ���������� �޼ҵ� */
	void InitVertexLayout();
	void InitRasterizer();

	void UpdateVertexBuffer();
	void UpdateVertexShader();
	void UpdateIndexBuffer();
	void UpdateRasterizer();	
	void UpdatePixelShader();
	void UpdateRenderTargetView();

	/** ������ ���������� ��� �޼ҵ� */
	virtual void InitShaderResourceView();
	virtual void UpdateShaderResourceView();

	virtual void InitVertexShader();
	virtual void InitPixelShader();

	/** UPrimitiveComponent �޼ҵ� */
	ENGINE_API void SetMesh(std::string_view MeshName);
	ENGINE_API void SetBlend(std::string_view BlendName);
	ENGINE_API void AddUVPlusValue(float4 Value);


	ENGINE_API void SetTexture(UTexture* NewTexture);
	ENGINE_API void SetTexture(std::string_view TextureName);
	ENGINE_API void SetSpriteData(UPaperSprite* PaperSprite, size_t Index);

	/** UObject ��� �޼ҵ� */
	ENGINE_API virtual void SetOrder(int NewOrder) override;

protected:
	/** UObject ��� �޼ҵ� */
	ENGINE_API virtual void BeginPlay() override;

	/** URenderer �޼ҵ� */
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

	UStaticMesh* Mesh = nullptr;
	UEngineBlend* Blend = nullptr;

	UTexture* Texture = nullptr;
	FPaperSpriteData SpriteData;

	FUVValue UVValueData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> UVValue = nullptr; // �������

protected:

private:

};