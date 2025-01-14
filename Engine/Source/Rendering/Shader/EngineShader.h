#pragma once
#include "EngineEnums.h"
#include "Core/Public/Misc/FileHelper.h"

#include "Rendering/Shader/EngineShaderResource.h"

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

	ENGINE_API static void ReflectionCompile(FFileHelper& FileHelper);

	UEngineShaderResource ShaderResource;

	Microsoft::WRL::ComPtr<ID3DBlob> GetShaderCodeBlob()
	{
		return ShaderCodeBlob;
	}

protected:
	void ShaderResCheck();

	EShaderType ShaderType = EShaderType::MAX;
	UINT VersionHigh = 5;
	UINT VersionLow = 0;
	Microsoft::WRL::ComPtr<ID3DBlob> ShaderCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> ErrorCodeBlob = nullptr; // 중간 컴파일 결과물
	std::string EntryName;

private:

};

