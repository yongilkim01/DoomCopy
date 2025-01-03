#pragma once
#include <Windows.h>

#include "EngineShader.h"
#include "EngineInputLayoutInfo.h"
#include "Classes/Engine/RenderAsset.h"
#include "Core/Misc/Paths.h"

/**
 *	����
 */
class UEngineVertexShader : public URenderAsset, public UEngineShader
{
public:
	/** ������, �Ҹ��� */
	UEngineVertexShader();
	~UEngineVertexShader();

	/** ��ü �� ���� ���� */
	UEngineVertexShader(const UEngineVertexShader& Other) = delete;
	UEngineVertexShader(UEngineVertexShader&& Other) noexcept = delete;
	UEngineVertexShader& operator=(const UEngineVertexShader& Other) = delete;
	UEngineVertexShader& operator=(UEngineVertexShader&& Other) noexcept = delete;

	static std::shared_ptr<UEngineVertexShader> Load(
		std::string_view _Path, 
		const std::string_view _EntryPoint, 
		UINT _VersionHigh = 5, 
		UINT _VersionLow = 0)
	{
		FPaths EnginePath = FPaths(_Path);
		std::string FileName = EnginePath.GetFileName();
		return Load(FileName, _Path, _EntryPoint, _VersionHigh, _VersionLow);
	}
	ENGINE_API static std::shared_ptr<UEngineVertexShader> Load(
		std::string_view _Name, 
		std::string_view _Path, 
		const std::string_view _EntryPoint, 
		UINT _VersionHigh = 5, 
		UINT _VersionLow = 0);

	ENGINE_API void Update();

protected:

private:
	ENGINE_API void AssetLoad();
	Microsoft::WRL::ComPtr<ID3D11VertexShader> ShaderRes = nullptr;
};

