#pragma once
#include "Rendering/Public/Shader/EngineShaderResource.h"
#include "Engine/Classes/Engine/Mesh.h"
#include "Engine/Public/Materials/Material.h"
#include "Engine/Public/Global/EngineEnums.h"

class UCameraComponent;
class UPrimitiveComponent;
class UTexture;
class UTransformObject;

/**
 *	����
 */
class URenderAsset
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API URenderAsset();
	ENGINE_API ~URenderAsset();

	ENGINE_API virtual void Render(UCameraComponent* _Camera, float _DeltaTime);

	ENGINE_API void MaterialResourceCheck();

	template<typename Data>
	ENGINE_API void ConstantBufferLinkData(std::string_view _Name, Data& _Data)
	{
		ConstantBufferLinkData(_Name, reinterpret_cast<void*>(&_Data));
	}

	ENGINE_API void ConstantBufferLinkData(std::string_view Name, void* _Data);

	ENGINE_API void Reset();

	/** ��, �� �޼ҵ� */
	ENGINE_API void SetMesh(std::string_view MeshName);
	ENGINE_API void SetMaterial(std::string_view MaterialName);
	ENGINE_API void SetTexture(std::string_view TextureName, std::string_view AssetName);
	ENGINE_API void SetTexture(std::string_view TextureName, UTexture* Texture);
	ENGINE_API void SetTexture(std::string_view TextureName, std::shared_ptr<UTexture> InTexture);
	ENGINE_API void SetSampler(std::string_view SamplerName, std::string_view AssetName);

	ENGINE_API void SetPrimitiveComponent(UPrimitiveComponent* PrimitiveComponent)
	{
		ParentPrimitiveComponent = PrimitiveComponent;
	}
	ENGINE_API std::shared_ptr<UMesh> GetMesh()
	{
		return Mesh;
	}
	ENGINE_API void SetMesh(std::shared_ptr<UMesh> NewMesh)
	{
		Mesh = NewMesh;
	}
	ENGINE_API std::shared_ptr<UEngineMaterial> GetMatrial()
	{
		return Material;
	}
	ENGINE_API void SetMaterial(std::shared_ptr<UEngineMaterial> NewMaterial)
	{
		Material = NewMaterial;
	}
	ENGINE_API UTransformObject* GetTransformObject()
	{
		return TransformObject;
	}
	ENGINE_API void SetTransformObject(UTransformObject* InTransformObject)
	{
		TransformObject = InTransformObject;
	}

	std::map<EShaderType, UEngineShaderResource> ShaderResourceMap;

protected:

private:
	void InputLayOutCreate();
	
	UPrimitiveComponent* ParentPrimitiveComponent = nullptr;
	UTransformObject* TransformObject = nullptr;

	/** ������ ��� �ʵ� */
	std::shared_ptr<UMesh> Mesh;
	std::shared_ptr<UEngineMaterial> Material;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;

};

