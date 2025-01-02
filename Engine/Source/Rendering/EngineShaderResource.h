#pragma once
#include "EngineConstantBuffer.h"

class UEngineConstantBufferRes
{
public:
	void* Data; // 자신에게 세팅될 데이터는 스스로 가지고 있을 것이다.
	UINT BufferSize;
	std::shared_ptr<UEngineConstantBuffer> Res;
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

	/** 객체 값 복사 방지 */
	UEngineShaderResource(const UEngineShaderResource& Other) = delete;
	UEngineShaderResource(UEngineShaderResource&& Other) noexcept = delete;
	UEngineShaderResource& operator=(const UEngineShaderResource& Other) = delete;
	UEngineShaderResource& operator=(UEngineShaderResource&& Other) noexcept = delete;

	void CreateConstantBufferRes(std::string_view Name, UEngineConstantBufferRes Res);

protected:

private:
	std::map<std::string, UEngineConstantBufferRes> ConstantBufferRess;
};

