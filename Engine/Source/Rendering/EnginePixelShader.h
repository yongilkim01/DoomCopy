#pragma once

/**
 *	설명
 */
class UEnginePixelShader
{
public:
	/** 생성자, 소멸자 */
	UEnginePixelShader();
	~UEnginePixelShader();

	/** 객체 값 복사 방지 */
	UEnginePixelShader(const UEnginePixelShader& Other) = delete;
	UEnginePixelShader(UEnginePixelShader&& Other) noexcept = delete;
	UEnginePixelShader& operator=(const UEnginePixelShader& Other) = delete;
	UEnginePixelShader& operator=(UEnginePixelShader&& Other) noexcept = delete;

protected:

private:

};

