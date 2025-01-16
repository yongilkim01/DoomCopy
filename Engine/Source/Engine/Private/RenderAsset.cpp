#include "pch.h"
#include "Engine/Classes/Engine/RenderAsset.h"

#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"
#include "Engine/Classes/Engine/Mesh.h"
#include "Engine/Classes/Engine/TransformObject.h"

#include "Engine/Public/Materials/Material.h"

#include "Rendering/Buffer/EngineInputLayoutInfo.h"

URenderAsset::URenderAsset()
{
}

URenderAsset::~URenderAsset()
{
}

void URenderAsset::Render(UCameraComponent* CameraComponent, float DetlaTime)
{
	for (std::pair<const EShaderType, UEngineShaderResource>& Pair : ShaderResourceMap)
	{
		Pair.second.Update();
	}

	Mesh->GetVertexBuffer()->Update();
	Material->GetVertexShader()->Update();
	Mesh->GetIndexBuffer()->Update();
	Material->UpdatePrimitiveTopology();

	UGameEngine::GetDeviceContext()->IASetInputLayout(InputLayout.Get());

	Material->GetRasterizerState()->Update();

	Material->GetPixelShader()->Update();

	Material->GetBlend()->Update();

	Material->GetDepthStencilState()->Update();

	UGameEngine::GetDeviceContext()->DrawIndexed(Mesh->GetIndexBuffer()->GetIndexCount(), 0, 0);
}

void URenderAsset::SetMesh(std::string_view Name)
{
	Mesh = UMesh::Find<UMesh>(Name);

	if (nullptr == Mesh)
	{
		MSGASSERT("메시 데이터가 존재하지 않습니다");
	}
	if (nullptr != Material)
	{
		InputLayOutCreate();
	}
}

void URenderAsset::SetMaterial(std::string_view _Name)
{
	Material = UEngineMaterial::Find<UEngineMaterial>(_Name);

	if (nullptr == Material)
	{
		MSGASSERT("마테리얼 데이터가 존재하지 않습니다");
	}

	MaterialResourceCheck();

	if (nullptr != Mesh)
	{
		InputLayOutCreate();
	}
}

void URenderAsset::SetTexture(std::string_view Name, std::string_view AssetName)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == ShaderResourceMap.contains(i))
		{
			continue;
		}
		if (false == ShaderResourceMap[i].IsTexture(Name))
		{
			continue;
		}
		ShaderResourceMap[i].TextureSetting(Name, AssetName);
	}
}

void URenderAsset::SetTexture(std::string_view TextureName, UTexture* Texture)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == ShaderResourceMap.contains(i))
		{
			continue;
		}
		if (false == ShaderResourceMap[i].IsTexture(TextureName))
		{
			continue;
		}
		ShaderResourceMap[i].TextureSetting(TextureName, Texture);
	}
}

void URenderAsset::SetTexture(std::string_view TextureName, std::shared_ptr<UTexture> InTexture)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == ShaderResourceMap.contains(i))
		{
			continue;
		}
		if (false == ShaderResourceMap[i].IsTexture(TextureName))
		{
			continue;
		}

		ShaderResourceMap[i].TextureSetting(TextureName, InTexture);
	}
}

void URenderAsset::SetSampler(std::string_view Name, std::string_view AssetName)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == ShaderResourceMap.contains(i))
		{
			continue;
		}
		if (false == ShaderResourceMap[i].IsSampler(Name))
		{
			continue;
		}
		ShaderResourceMap[i].SamplerSetting(Name, AssetName);
	}
}

void URenderAsset::MaterialResourceCheck()
{
	if (nullptr == Material)
	{
		MSGASSERT("존재하지 않는 마테리얼입니다.");
		return;
	}

	UEngineShaderResource& VertexShader = Material->GetVertexShader()->ShaderResource;
	ShaderResourceMap[EShaderType::VS] = Material->GetVertexShader()->ShaderResource;

	UEngineShaderResource& PixelShader = Material->GetPixelShader()->ShaderResource;
	ShaderResourceMap[EShaderType::PS] = Material->GetPixelShader()->ShaderResource;

	if (nullptr != ParentPrimitiveComponent)
	{
		TransformObject = ParentPrimitiveComponent;
	}

	if(nullptr != TransformObject)
	{
		for (EShaderType i = EShaderType::VS; i < EShaderType::MAX; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
		{
			if (false == ShaderResourceMap.contains(i))
			{
				continue;
			}
			if (false == ShaderResourceMap[i].IsConstantBuffer("FTransform"))
			{
				continue;
			}

			FTransform& Ref = TransformObject->GetComponentTransformRef();
			ShaderResourceMap[i].ConstantBufferLinkData("FTransform", Ref);
		}

	}

}

void URenderAsset::ConstantBufferLinkData(std::string_view Name, void* _Data)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == ShaderResourceMap.contains(i))
		{
			continue;
		}
		if (false == ShaderResourceMap[i].IsConstantBuffer(Name))
		{
			continue;
		}

		ShaderResourceMap[i].ConstantBufferLinkData(Name, _Data);
	}
}

void URenderAsset::Reset()
{
	for (std::pair<const EShaderType, UEngineShaderResource>& Pair : ShaderResourceMap)
	{
		Pair.second.Reset();
	}
}

void URenderAsset::InputLayOutCreate()
{
	Microsoft::WRL::ComPtr<ID3DBlob> Blob = Material->GetVertexShader()->GetShaderCodeBlob();
	UEngineInputLayoutInfo* InfoPtr = Mesh->GetVertexBuffer()->GetInfoPtr();
	HRESULT Result = UGameEngine::GetDevice()->CreateInputLayout(
		&InfoPtr->InputLayoutData[0],
		static_cast<unsigned int>(InfoPtr->InputLayoutData.size()),
		Blob->GetBufferPointer(),
		Blob->GetBufferSize(),
		&InputLayout);
}
