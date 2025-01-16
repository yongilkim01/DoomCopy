#pragma once
#include "Engine/Classes/Engine/EngineResource.h"
#include "EngineEnums.h"

/**
 *	����
 */
class UEngineSampler : public UResource
{
public:
	/** ������, �Ҹ��� */
	UEngineSampler();
	~UEngineSampler();

	/** ��ü �� ���� ���� */
	UEngineSampler(const UEngineSampler& Other) = delete;
	UEngineSampler(UEngineSampler&& Other) noexcept = delete;
	UEngineSampler& operator=(const UEngineSampler& Other) = delete;
	UEngineSampler& operator=(UEngineSampler&& Other) noexcept = delete;

	static std::shared_ptr<UEngineSampler> Create(std::string_view Name, const D3D11_SAMPLER_DESC& Value);

	void Update(EShaderType ShaderType, UINT BindIndex);
	void Reset(EShaderType Type, UINT BindIndex);

protected:
	void AssetCreate(const D3D11_SAMPLER_DESC& SamplerDesc);

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> State = nullptr;


};