#include "Engine.h"
#include "Rendering/Public/Shader/EnginePixelShader.h"

UEnginePixelShader::UEnginePixelShader()
{
	ShaderType = EShaderType::PS;
}

UEnginePixelShader::~UEnginePixelShader()
{
}

std::shared_ptr<UEnginePixelShader> UEnginePixelShader::Load(std::string_view _Name, std::string_view _Path, const std::string_view _EntryPoint, UINT _VersionHigh, UINT _VersionLow)
{
	std::string UpperName = ToUpperName(_Name);
	if (true == Contains(UpperName))
	{
		MSGASSERT("�̹� �ε��� �ȼ� ���̴��� �ε��Ϸ��� �߽��ϴ�" + UpperName);
		return nullptr;
	}
	std::shared_ptr<UEnginePixelShader> NewRes = std::make_shared<UEnginePixelShader>();
	AddAsset<UEnginePixelShader>(NewRes, _Name, _Path);
	NewRes->VersionHigh = _VersionHigh;
	NewRes->VersionLow = _VersionLow;
	NewRes->EntryName = _EntryPoint;
	NewRes->AssetLoad();
	return NewRes;
}

void UEnginePixelShader::Update()
{
	UGameEngine::GetDeviceContext()->PSSetShader(ShaderRes.Get(), nullptr, 0);

}

void UEnginePixelShader::AssetLoad()
{
	std::wstring WPath = UEngineString::AnsiToUnicode(GetPath().GetPathToString());
	std::string version = "ps_" + std::to_string(VersionHigh) + "_" + std::to_string(VersionLow);

	int Flag0 = 0;
	int Flag1 = 0;

#ifdef _DEBUG
	Flag0 = D3D10_SHADER_DEBUG;
#endif
	Flag0 |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
	D3DCompileFromFile(
		WPath.c_str(),
		nullptr, // Define TEST ������ ��ó���⸦ ������.
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		EntryName.c_str(),
		version.c_str(),
		Flag0,
		Flag1,
		&ShaderCodeBlob,
		&ErrorCodeBlob
	);

	if (nullptr == ShaderCodeBlob)
	{
		std::string ErrString = reinterpret_cast<char*>(ErrorCodeBlob->GetBufferPointer());
		MSGASSERT("���̴� �ڵ� �߰����忡�� �����߽��ϴ�\n" + ErrString);
		return;
	}

	HRESULT Result = UGameEngine::GetDevice()->CreatePixelShader(
		ShaderCodeBlob->GetBufferPointer(),
		ShaderCodeBlob->GetBufferSize(),
		nullptr,
		&ShaderRes
	);

	if (S_OK != Result)
	{
		MSGASSERT("�ȼ� ���̴� ������ �����߽��ϴ�.");
	}

	UEngineShader::ShaderResCheck();
}

