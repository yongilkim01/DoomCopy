#pragma once
#include "Classes/Components/SceneComponent.h"

class ULevel;
class UCameraComponent;

struct EngineVertex
{
	float4 POSITION;
	float4 TEXCOORD; // UV값이라고 불리는 존재로 텍스처가 매핑되는 비율을 지정해줍니다.
	float4 COLOR;
};
/**
 *	렌더러 클래스
 */
class URenderer : public USceneComponent
{
	friend ULevel;
	friend UCameraComponent;

public:
	/** UObject 상속 메소드 */
	ENGINE_API URenderer();
	ENGINE_API ~URenderer();

	/** 객체 값 복사 방지 */
	URenderer(const URenderer& Other) = delete;
	URenderer(URenderer&& Other) noexcept = delete;
	URenderer& operator=(const URenderer& Other) = delete;
	URenderer& operator=(URenderer&& Other) noexcept = delete;

	/** 초기화 메소드 */
	void InputAssembler1Init();
	void InputAssembler1Setting();
	void InputAssembler1Layout();

	void VertexShaderInit();
	void VertexShaderSetting();

	void InputAssembler2Init();
	void InputAssembler2Setting();

	void RasterizerInit();
	void RasterizerSetting();

	void PixelShaderInit();
	void PixelShaderSetting();

	void OutPutMergeSetting();

	void ShaderResInit();
	void ShaderResSetting();

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

	Microsoft::WRL::ComPtr<ID3D11Buffer> TransformConstBuffer = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState = nullptr;
};