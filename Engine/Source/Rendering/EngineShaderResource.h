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
	void* Data = nullptr; // �ڽſ��� ���õ� �����ʹ� ������ ������ ���� ���̴�.
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
 *	����
 */
class UEngineShaderResource
{
public:
	/** ������, �Ҹ��� */
	UEngineShaderResource();
	~UEngineShaderResource();

	void CreateConstantBufferRes(std::string_view Name, UEngineConstantBufferRes Res);

	void Update();

protected:

private:
	std::map<std::string, UEngineConstantBufferRes> ConstantBufferRes;
};

