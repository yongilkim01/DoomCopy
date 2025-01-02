#pragma once
#include "EngineShaderResource.h"
#include "Classes/Engine/Mesh.h"
#include "Rendering/EngineMaterial.h"

/**
 *	����
 */
class URenderUnit
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API URenderUnit();
	ENGINE_API ~URenderUnit();

	// �޽�(��ü) 
	std::shared_ptr<UMesh> Mesh;
	// ��Ƽ����(�Ǻ�)
	std::shared_ptr<UEngineMaterial> Material;

	void SetMesh(std::string_view _Name);

	void SetMaterial(std::string_view _Name);

	ENGINE_API virtual void Render(class UEngineCamera* _Camera, float _DeltaTime);

protected:

private:
	UEngineConstantBufferRes Res;

};

