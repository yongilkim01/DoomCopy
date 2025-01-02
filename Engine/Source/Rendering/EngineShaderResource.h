#pragma once
#include "EngineConstantBuffer.h"

class UEngineConstantBufferRes
{
public:
	void* Data; // �ڽſ��� ���õ� �����ʹ� ������ ������ ���� ���̴�.
	UINT BufferSize;
	std::shared_ptr<UEngineConstantBuffer> Res;
};

/**
 *	����
 */
class UEngineShaderResource
{
public:
	/** ������, �Ҹ��� */
	UEngineShaderResource();
	~UEngineShaderResource();

	/** ��ü �� ���� ���� */
	UEngineShaderResource(const UEngineShaderResource& Other) = delete;
	UEngineShaderResource(UEngineShaderResource&& Other) noexcept = delete;
	UEngineShaderResource& operator=(const UEngineShaderResource& Other) = delete;
	UEngineShaderResource& operator=(UEngineShaderResource&& Other) noexcept = delete;

	void CreateConstantBufferRes(std::string_view Name, UEngineConstantBufferRes Res);

protected:

private:
	std::map<std::string, UEngineConstantBufferRes> ConstantBufferRess;
};

