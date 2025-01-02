#pragma once
#include "EngineEnums.h"
#include "Core/Misc/FileHelper.h"

/**
 *	����
 */
class UEngineShader
{
public:
	/** ������, �Ҹ��� */
	UEngineShader();
	~UEngineShader();

	/** ��ü �� ���� ���� */
	UEngineShader(const UEngineShader& Other) = delete;
	UEngineShader(UEngineShader&& Other) noexcept = delete;
	UEngineShader& operator=(const UEngineShader& Other) = delete;
	UEngineShader& operator=(UEngineShader&& Other) noexcept = delete;

	static void ReflectionCompile(FFileHelper& FileHelper);

protected:

private:

};

