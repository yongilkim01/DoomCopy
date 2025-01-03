#pragma once
#include "EngineEnums.h"
#include "Core/Misc/FileHelper.h"

#include "EngineShaderResource.h"

class UEngineVertexShader;

/**
 *	설명
 */
class UEngineShader
{
	friend UEngineVertexShader;
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

	UEngineShaderResource ShaderResource;

	Microsoft::WRL::ComPtr<ID3DBlob> GetShaderCodeBlob()
	{
		return ShaderCodeBlob;
	}

protected:
	void ShaderResCheck();

	UINT VersionHigh = 5;
	UINT VersionLow = 0;
	Microsoft::WRL::ComPtr<ID3DBlob> ShaderCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> ErrorCodeBlob = nullptr; // 중간 컴파일 결과물
	std::string EntryName;

private:

};

