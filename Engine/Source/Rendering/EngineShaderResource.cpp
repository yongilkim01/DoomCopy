#include "pch.h"
#include "EngineShaderResource.h"
#include "Classes/Engine/Texture.h"
#include "WorldPartition/DebugHelpers.h"

UEngineShaderResource::UEngineShaderResource()
{
}

UEngineShaderResource::~UEngineShaderResource()
{
}

void UEngineShaderResource::Update()
{
	for (std::pair<const std::string, UEngineConstantBufferRes>& Res : ConstantBufferRes)
	{
		Res.second.Update();
	}
	for (std::pair<const std::string, UEngineTextureRes>& Res : TextureRes)
	{
		Res.second.Update();
	}
	for (std::pair<const std::string, UEngineSamplerRes>& Res : SamplerRes)
	{
		Res.second.Update();
	}
}

void UEngineShaderResource::CreateConstantBufferRes(std::string_view Name, UEngineConstantBufferRes ConstantBufferResource)
{
	std::string UpperString = UEngineString::ToUpper(Name);

	if (true == ConstantBufferRes.contains(UpperString))
	{
		MSGASSERT("같은 이름 상수버퍼가 한 셰이더에 2개가 존재합니다");
		return;
	}
	ConstantBufferRes[UpperString] = ConstantBufferResource;
}

void UEngineShaderResource::CreateSamplerRes(std::string_view Name, UEngineSamplerRes SamplerResource)
{
	std::string UpperString = UEngineString::ToUpper(Name);

	if (true == SamplerRes.contains(UpperString))
	{
		MSGASSERT("같은 이름 상수버퍼가 한 쉐이더에 2개가 존재합니다");
		return;
	}
	SamplerRes[UpperString] = SamplerResource;
}

void UEngineShaderResource::CreateTextureRes(std::string_view Name, UEngineTextureRes TextureResource)
{
	std::string UpperString = UEngineString::ToUpper(Name);

	if (true == TextureRes.contains(UpperString))
	{
		MSGASSERT("같은 이름 상수버퍼가 한 쉐이더에 2개가 존재합니다");
		return;
	}

	TextureRes[UpperString] = TextureResource;
}

void UEngineShaderResource::ConstantBufferLinkData(std::string_view Name, void* Data)
{
	std::string UpperName = UEngineString::ToUpper(Name);

	if (false == ConstantBufferRes.contains(UpperName))
	{
		UEngineDebug::OutPutString("ConstantBufferRes.contains " + UpperName);
		return;
	}

	ConstantBufferRes[UpperName].Data = Data;
}

void UEngineShaderResource::SamplerSetting(std::string_view Name, std::string_view ResourceName)
{
	std::string UpperName = UEngineString::ToUpper(Name);
	if (false == SamplerRes.contains(UpperName))
	{
		UEngineDebug::OutPutString("ConstantBufferRes.contains " + UpperName);
		return;
	}
	SamplerRes[UpperName].Res = UEngineSampler::Find<UEngineSampler>(ResourceName);
}

void UEngineShaderResource::TextureSetting(std::string_view Name, std::string_view ResourceName)
{
	std::string UpperName = UEngineString::ToUpper(Name);
	if (false == TextureRes.contains(UpperName))
	{
		UEngineDebug::OutPutString("ConstantBufferRes.contains " + UpperName);
		return;
	}
	TextureRes[UpperName].Res = UTexture::Find<UTexture>(ResourceName);
}

bool UEngineShaderResource::IsSampler(std::string_view Name)
{
	std::string UpperName = UEngineString::ToUpper(Name);

	return SamplerRes.contains(UpperName);
}

bool UEngineShaderResource::IsTexture(std::string_view Name)
{
	std::string UpperName = UEngineString::ToUpper(Name);

	return TextureRes.contains(UpperName);
}

bool UEngineShaderResource::IsConstantBuffer(std::string_view Name)
{
	std::string UpperName = UEngineString::ToUpper(Name);

	return ConstantBufferRes.contains(UpperName);
}