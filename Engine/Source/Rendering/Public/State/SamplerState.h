#pragma once
#include "Engine/Classes/Engine/EngineResource.h"
#include "Engine/Public/Global/EngineEnums.h"

/**
 *	����
 */
class USamplerState : public UResource
{
public:
	/** ������, �Ҹ��� */
	USamplerState();
	~USamplerState();

	/** ��ü �� ���� ���� */
	USamplerState(const USamplerState& Other) = delete;
	USamplerState(USamplerState&& Other) noexcept = delete;
	USamplerState& operator=(const USamplerState& Other) = delete;
	USamplerState& operator=(USamplerState&& Other) noexcept = delete;

	static std::shared_ptr<USamplerState> Create(std::string_view Name, const D3D11_SAMPLER_DESC& Value);

	void Update(EShaderType ShaderType, UINT BindIndex);
	void Reset(EShaderType Type, UINT BindIndex);

protected:
	void AssetCreate(const D3D11_SAMPLER_DESC& SamplerDesc);

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> State = nullptr;


};