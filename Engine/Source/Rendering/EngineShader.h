#pragma once
#include "EngineEnums.h"
#include "Core/Misc/FileHelper.h"

/**
 *	설명
 */
class UEngineShader
{
public:
	/** 생성자, 소멸자 */
	UEngineShader();
	~UEngineShader();

	/** 객체 값 복사 방지 */
	UEngineShader(const UEngineShader& Other) = delete;
	UEngineShader(UEngineShader&& Other) noexcept = delete;
	UEngineShader& operator=(const UEngineShader& Other) = delete;
	UEngineShader& operator=(UEngineShader&& Other) noexcept = delete;

	static void ReflectionCompile(FFileHelper& FileHelper);

protected:

private:

};

