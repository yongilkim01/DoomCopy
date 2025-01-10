#pragma once
#include "Rendering/Buffer/EngineConstantBuffer.h"
#include "Core/Object/Object.h"
#include "EngineEnums.h"
#include "Classes/Engine/Texture.h"
#include "Rendering/Sampler/EngineSampler.h"

class UEngineShaderRes
{
public:
	std::string Name;
	EShaderType ShaderType = EShaderType::MAX;
	UINT BindIndex = 0;
};

class UEngineConstantBufferRes : public UEngineShaderRes
{
public:
	void* Data = nullptr; // 자신에게 세팅될 데이터는 스스로 가지고 있을 것이다.
	UINT BufferSize;
	std::shared_ptr<UEngineConstantBuffer> Res;

	void Update()
	{
		if (nullptr != Data)
		{
			Res->ChangeData(Data, BufferSize);
		}
		Res->Update(ShaderType, BindIndex);
	}
};

class UEngineTextureRes : public UEngineShaderRes
{
public:
	UTexture* Res;
	void Update()
	{
		Res->Update(ShaderType, BindIndex);
	}
};

class UEngineSamplerRes : public UEngineShaderRes
{
public:
	std::shared_ptr<UEngineSampler> Res;

	void Update()
	{
		Res->Update(ShaderType, BindIndex);
	}

};

/**
 *	설명
 */
class UEngineShaderResource
{
public:
	/** 생성자, 소멸자 */
	UEngineShaderResource();
	~UEngineShaderResource();

	void CreateConstantBufferRes(std::string_view Name, UEngineConstantBufferRes Res);
	void CreateSamplerRes(std::string_view Name, UEngineSamplerRes SamplerResource);
	void CreateTextureRes(std::string_view Name, UEngineTextureRes TextureResource);

	template<typename DataType>
	void ConstantBufferLinkData(std::string_view Name, DataType& Data)
	{
		ConstantBufferLinkData(Name, reinterpret_cast<void*>(&Data));
	}
	void ConstantBufferLinkData(std::string_view Name, void* Data);

	void SamplerSetting(std::string_view Name, std::string_view ResourceName);
	void TextureSetting(std::string_view Name, std::string_view ResourceName);
	void TextureSetting(std::string_view Name, UTexture* Texture);

	bool IsSampler(std::string_view Name);
	bool IsTexture(std::string_view Name);
	bool IsConstantBuffer(std::string_view Name);

	void Update();

	int Index = 0;
protected:

private:
	std::map<std::string, UEngineConstantBufferRes> ConstantBufferRes;
	std::map<std::string, UEngineTextureRes> TextureRes;
	std::map<std::string, UEngineSamplerRes> SamplerRes;
};

