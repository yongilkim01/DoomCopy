#pragma once
#include "Classes/Engine/RenderAsset.h"
#include "Rendering/Shader/EngineVertexShader.h"
#include "Rendering/Shader/EnginePixelShader.h"
#include "EngineRasterizerState.h"
#include "EngineBlend.h"

/**
 *	설명
 */
class UEngineMaterial : public URenderAsset
{
public:
	/** 생성자, 소멸자 */
	UEngineMaterial();
	~UEngineMaterial();

	/** 객체 값 복사 방지 */
	UEngineMaterial(const UEngineMaterial& Other) = delete;
	UEngineMaterial(UEngineMaterial&& Other) noexcept = delete;
	UEngineMaterial& operator=(const UEngineMaterial& Other) = delete;
	UEngineMaterial& operator=(UEngineMaterial&& Other) noexcept = delete;

	ENGINE_API static std::shared_ptr<UEngineMaterial> Create(std::string_view _Name);
	ENGINE_API void UpdatePrimitiveTopology();

	ENGINE_API std::shared_ptr<UEngineVertexShader> GetVertexShader()
	{
		return VertexShader;
	}
	ENGINE_API void SetVertexShader(std::string_view _Name);
	ENGINE_API std::shared_ptr<UEnginePixelShader> GetPixelShader()
	{
		return PixelShader;
	}
	ENGINE_API void SetPixelShader(std::string_view _Name);
	ENGINE_API std::shared_ptr<UEngineRasterizerState> GetRasterizerState()
	{
		return RasterizerState;
	}
	ENGINE_API void SetRasterizerState(std::string_view _Name);
	ENGINE_API std::shared_ptr<UEngineBlend> GetBlend()
	{
		return Blend;
	}
	ENGINE_API void SetBlend(std::string_view _Name);

protected:

private:
	std::shared_ptr<UEngineVertexShader> VertexShader;
	std::shared_ptr<UEnginePixelShader> PixelShader;
	std::shared_ptr<UEngineRasterizerState> RasterizerState;
	std::shared_ptr<UEngineBlend> Blend;

	D3D11_PRIMITIVE_TOPOLOGY TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

};

