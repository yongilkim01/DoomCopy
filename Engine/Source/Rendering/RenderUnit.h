#pragma once
#include "EngineShaderResource.h"
#include "Classes/Engine/Mesh.h"
#include "Rendering/EngineMaterial.h"

/**
 *	설명
 */
class URenderUnit
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API URenderUnit();
	ENGINE_API ~URenderUnit();

	// 메쉬(육체) 
	std::shared_ptr<UMesh> Mesh;
	// 머티리얼(피부)
	std::shared_ptr<UEngineMaterial> Material;

	void SetMesh(std::string_view _Name);

	void SetMaterial(std::string_view _Name);

	ENGINE_API virtual void Render(class UEngineCamera* _Camera, float _DeltaTime);

protected:

private:
	UEngineConstantBufferRes Res;

};

