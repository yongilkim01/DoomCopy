#pragma once
#include <Windows.h>

#include "EngineShader.h"
#include "Engine/Classes/Engine/RenderAsset.h"
#include "Core/Misc/Paths.h"
#include "EngineEnums.h"

/**
 *	설명
 */
class UEnginePixelShader : public URenderAsset, public UEngineShader
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

	static std::shared_ptr<UEnginePixelShader> Load(
		std::string_view _Path,
		const std::string_view _EntryPoint,
		UINT _VersionHigh = 5,
		UINT _VersionLow = 0)
	{
		FPaths EnginePath = FPaths(_Path);
		std::string FileName = EnginePath.GetFileName();
		return Load(FileName, _Path, _EntryPoint, _VersionHigh, _VersionLow);
	}
	ENGINE_API static std::shared_ptr<UEnginePixelShader> Load(
		std::string_view _Name,
		std::string_view _Path,
		const std::string_view _EntryPoint,
		UINT _VersionHigh = 5,
		UINT _VersionLow = 0);

	ENGINE_API void Update();

protected:

private:
	ENGINE_API void AssetLoad();
	Microsoft::WRL::ComPtr<ID3D11PixelShader> ShaderRes = nullptr;
};

