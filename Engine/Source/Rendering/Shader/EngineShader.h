#pragma once
#include "EngineEnums.h"
#include "Core/Public/Misc/FileHelper.h"

#include "Rendering/Shader/EngineShaderResource.h"

class UEngineVertexShader;

/**
 *	����
 */
class UEngineShader
{
	friend UEngineVertexShader;
public:
	/** ������, �Ҹ��� */
	UEngineShader();
	~UEngineShader();

	/** ��ü �� ���� ���� */
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
	Microsoft::WRL::ComPtr<ID3DBlob> ErrorCodeBlob = nullptr; // �߰� ������ �����
	std::string EntryName;

private:

};

