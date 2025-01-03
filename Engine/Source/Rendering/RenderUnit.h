#pragma once
#include "EngineShaderResource.h"
#include "Classes/Engine/Mesh.h"
#include "Rendering/EngineMaterial.h"
#include "EngineEnums.h"
#include "EngineShaderResource.h"

class UCameraComponent;

/**
 *	설명
 */
class URenderUnit
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API URenderUnit();
	ENGINE_API ~URenderUnit();

	ENGINE_API virtual void Render(UCameraComponent* _Camera, float _DeltaTime);


	// 메쉬(육체) 
	std::shared_ptr<UMesh> Mesh;
	// 머티리얼(피부)
	std::shared_ptr<UEngineMaterial> Material;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;

	ENGINE_API void SetMesh(std::string_view _Name);
	ENGINE_API void SetMaterial(std::string_view _Name);

	ENGINE_API void MaterialResourceCheck();

protected:

private:
	void InputLayOutCreate();
	
	std::map<EShaderType, UEngineShaderResource> ShaderResourceMap;
};

