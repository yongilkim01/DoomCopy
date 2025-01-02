#pragma once

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

protected:

private:

};

