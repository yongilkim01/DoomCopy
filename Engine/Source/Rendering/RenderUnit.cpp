#include "pch.h"
#include "RenderUnit.h"

#include "Classes/Camera/CameraComponent.h"

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
	Mesh->GetVertexBuffer()->Update();
	Material->GetVertexShader()->Update();
	Mesh->GetIndexBuffer()->Update();
	Material->UpdatePrimitiveTopology();

	UEngineCore::GetDevice().GetDeviceContext()->IASetInputLayout(InputLayOut.Get());

	Material->GetRasterizerState()->Update();

	Material->GetPixelShader()->Update();

	Material->GetBlend()->Update();

	ID3D11RenderTargetView* RTV = UEngineCore::GetDevice().GetRenderTargetView();
	ID3D11RenderTargetView* ArrRTV[16] = { 0 };
	ArrRTV[0] = RTV;
	
	UEngineCore::GetDevice().GetDeviceContext()->OMSetRenderTargets(1, &ArrRTV[0], nullptr);
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

	if (nullptr != Material)
	{
		MSGASSERT("마테리얼 데이터가 존재하지 않습니다");
	}
	if (nullptr != Mesh)
	{
		InputLayOutCreate();
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
