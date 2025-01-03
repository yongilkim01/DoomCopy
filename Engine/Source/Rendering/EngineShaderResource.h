#pragma once
#include "EngineConstantBuffer.h"
#include "Core/Object/Object.h"
#include "EngineEnums.h"

class UEngineShaderRes
{
public:
	std::string Name;
	EShaderType Type = EShaderType::MAX;
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
			Res->ChangeData(Data, Res->GetBufferInfo().ByteWidth);
		}
		Res->Update(Type, BindIndex);
	}
};

//class UEngineSamplerRes : public UEngineShaderRes
//{
//public:
//	std::shared_ptr<UEngineSampler> Res;
//
//	void Setting()
//	{
//		Res->Setting(ShaderType, BindIndex);
//	}
//
//};

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

	void Update();

protected:

private:
	std::map<std::string, UEngineConstantBufferRes> ConstantBufferRes;
};

