#include "pch.h"
#include "RenderUnit.h"

#include "Classes/Camera/CameraComponent.h"
#include "Classes/Components/PrimitiveComponent.h"

#include "Classes/Engine/Mesh.h"
#include "Rendering/EngineMaterial.h"
#include "Rendering/EngineInputLayoutInfo.h"

URenderUnit::URenderUnit()
{
}

URenderUnit::~URenderUnit()
{
}

void URenderUnit::Render(UCameraComponent* CameraComponent, float DetlaTime)
{
	for (std::pair<const EShaderType, UEngineShaderResource>& Pair : ShaderResourceMap)
	{
		Pair.second.Update();
	}

	Mesh->GetVertexBuffer()->Update();
	Material->GetVertexShader()->Update();
	Mesh->GetIndexBuffer()->Update();
	Material->UpdatePrimitiveTopology();

	UEngineCore::GetDevice().GetDeviceContext()->IASetInputLayout(InputLayout.Get());

	Material->GetRasterizerState()->Update();

	Material->GetPixelShader()->Update();

	Material->GetBlend()->Update();

	UEngineCore::GetDevice().GetDeviceContext()->DrawIndexed(Mesh->GetIndexBuffer()->GetIndexCount(), 0, 0);
}

void URenderUnit::SetMesh(std::string_view Name)
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

void URenderUnit::SetMaterial(std::string_view _Name)
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

void URenderUnit::SetTexture(std::string_view Name, std::string_view AssetName)
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

void URenderUnit::SetSampler(std::string_view Name, std::string_view AssetName)
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

void URenderUnit::MaterialResourceCheck()
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

			FTransform& Ref = ParentPrimitiveComponent->GetComponentTransformRef();
			ShaderResourceMap[i].ConstantBufferLinkData("FTransform", Ref);
		}

	}

}

void URenderUnit::ConstantBufferLinkData(std::string_view Name, void* _Data)
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

void URenderUnit::InputLayOutCreate()
{
	Microsoft::WRL::ComPtr<ID3DBlob> Blob = Material->GetVertexShader()->GetShaderCodeBlob();
	UEngineInputLayoutInfo* InfoPtr = Mesh->GetVertexBuffer()->GetInfoPtr();
	HRESULT Result = UEngineCore::GetDevice().GetDevice()->CreateInputLayout(
		&InfoPtr->InputLayoutData[0],
		static_cast<unsigned int>(InfoPtr->InputLayoutData.size()),
		Blob->GetBufferPointer(),
		Blob->GetBufferSize(),
		&InputLayout);
}
