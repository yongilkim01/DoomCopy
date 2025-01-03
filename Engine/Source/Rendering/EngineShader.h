#pragma once
#include "EngineEnums.h"
#include "Core/Misc/FileHelper.h"

#include "EngineShaderResource.h"

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
	Microsoft::WRL::ComPtr<ID3DBlob> ErrorCodeBlob = nullptr; // �߰� ������ �����
	std::string EntryName;

private:

};

