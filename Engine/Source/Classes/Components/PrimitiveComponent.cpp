#include "pch.h"
#include "PrimitiveComponent.h"

#include "Classes/Engine/Level.h"
#include "Classes/Engine/Texture.h"
#include "Classes/Camera/CameraComponent.h"
#include "Classes/Engine/PaperSprite.h"
#include "Classes/Engine/StaticMesh.h"

#include "Rendering/EngineVertex.h"
#include "Rendering/EngineBlend.h"

#include "Core/Misc/DirectoryHelper.h"
#include "Core/Misc/FileHelper.h"
#include "Core/Containers/EngineString.h"
#include "Core/EngineCore.h"

UPrimitiveComponent::UPrimitiveComponent()
{
}

UPrimitiveComponent::~UPrimitiveComponent()
{

}

void UPrimitiveComponent::BeginPlay()
{
	USceneComponent::BeginPlay();

	SetOrder(GetOrder());

	InitVertexShader();
	InitRasterizer();
	InitPixelShader();
	InitShaderResourceView();

	//SetMesh("Rect");
}

void UPrimitiveComponent::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	FTransform& CameraTransform = CameraComponent->GetComponentTransformRef();
	FTransform& RendererTransform = GetComponentTransformRef();

	RendererTransform.View = CameraTransform.View;
	RendererTransform.Projection = CameraTransform.Projection;

	RendererTransform.WVP = RendererTransform.World * RendererTransform.View * RendererTransform.Projection;

	if (nullptr == Mesh)
	{
		MSGASSERT("�޽��� ���õ��� �ʾҽ��ϴ�.");
		return;
	}

	UEngineCore::GetDevice().GetDeviceContext()->IASetPrimitiveTopology(Topology);


	// Rendering pipeline
	UpdateShaderResourceView();
	UpdateVertexBuffer();
	UpdateVertexShader();
	UpdateIndexBuffer();
	UpdateRasterizer();
	UpdatePixelShader();
	UpdateRenderTargetView();

	UEngineCore::GetDevice().GetDeviceContext()->DrawIndexed(6, 0, 0);


}

void UPrimitiveComponent::UpdateVertexBuffer()
{
	Mesh->GetVertexBuffer()->Update();
	UEngineCore::GetDevice().GetDeviceContext()->IASetInputLayout(InputLayout.Get());
}

void UPrimitiveComponent::InitVertexLayout()
{
	// Vertex �Է� ���̾ƿ��� ������ ���� ����
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutData;

	// POSITION �Է� ��� ���� �߰�
	{
		D3D11_INPUT_ELEMENT_DESC Desc;
		Desc.SemanticName = "POSITION"; // �ǹ̷� �̸�
		Desc.InputSlot = 0; // �Է� ����
		Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // ������ ����
		Desc.AlignedByteOffset = 0; // �������� ����Ʈ ������
		Desc.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA; // �Է� ���� Ŭ����
		Desc.SemanticIndex = 0; // �ǹ̷� �ε���
		Desc.InstanceDataStepRate = 0; // �ν��Ͻ� ������ �ܰ� �ӵ�
		InputLayoutData.push_back(Desc); // �Է� ���̾ƿ� �����Ϳ� �߰�
	}

	// TEXCOORD �Է� ��� ���� �߰�
	{
		D3D11_INPUT_ELEMENT_DESC Desc;
		Desc.SemanticName = "TEXCOORD"; // �ǹ̷� �̸�
		Desc.InputSlot = 0; // �Է� ����
		Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // ������ ����
		Desc.AlignedByteOffset = 16; // �������� ����Ʈ ������
		Desc.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA; // �Է� ���� Ŭ����

		// �ν��Ͻ� ����
		Desc.SemanticIndex = 0; // �ǹ̷� �ε���
		Desc.InstanceDataStepRate = 0; // �ν��Ͻ� ������ �ܰ� �ӵ�
		InputLayoutData.push_back(Desc); // �Է� ���̾ƿ� �����Ϳ� �߰�
	}

	// COLOR �Է� ��� ���� �߰�
	{
		D3D11_INPUT_ELEMENT_DESC Desc;
		Desc.SemanticName = "COLOR"; // �ǹ̷� �̸�
		Desc.InputSlot = 0; // �Է� ����
		Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // ������ ����
		Desc.AlignedByteOffset = 32; // �������� ����Ʈ ������
		Desc.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA; // �Է� ���� Ŭ����
		Desc.SemanticIndex = 0; // �ǹ̷� �ε���
		Desc.InstanceDataStepRate = 0; // �ν��Ͻ� ������ �ܰ� �ӵ�
		InputLayoutData.push_back(Desc); // �Է� ���̾ƿ� �����Ϳ� �߰�
	}

	// �Է� ���̾ƿ� ����
	HRESULT Result = UEngineCore::GetDevice().GetDevice()->CreateInputLayout(
		&InputLayoutData[0],                        // �Է� ���̾ƿ� ������ ������
		static_cast<unsigned int>(InputLayoutData.size()), // �Է� ���̾ƿ� ��� ����
		VSShaderCodeBlob->GetBufferPointer(),       // ���̴� �ڵ� ������
		VSShaderCodeBlob->GetBufferSize(),          // ���̴� �ڵ� ũ��
		&InputLayout);                              // ������ �Է� ���̾ƿ� ��ü

	// �Է� ���̾ƿ� ���� ���� �� ���� ó��
	if (S_OK != Result)
	{
		MSGASSERT("��ǲ ���̾ƿ� ���� ����");
	}

}

void UPrimitiveComponent::InitVertexShader()
{
	// ���� ���丮 ���� ��ü ����
	FDirectoryHelper CurDir;
	// ���� ���̴� ���丮�� �̵�
	CurDir.MoveEngineShaderDirectory();
	// ���̴� ������ ������
	FFileHelper VSFile = CurDir.GetFile("DefaultShader.fx");
	// ���� ��θ� ���ڿ��� ��ȯ
	std::string Path = VSFile.GetPathToString();
	// ���ڿ� ��θ� ���̵� ���ڿ��� ��ȯ
	std::wstring WPath = UEngineString::AnsiToUnicode(Path);
	// ���ؽ� ���̴� ���� ����
	std::string VSVersion = "vs_5_0";

	// �÷��� ���� �ʱ�ȭ
	int Flag0 = 0;
	int Flag1 = 0;

	// ����� ��忡�� ����� �÷��� ����
#ifdef _DEBUG
	Flag0 = D3D10_SHADER_DEBUG;
#endif

	// ����� �� �켱 ������� ��ŷ�ϴ� �÷��� ����
	Flag0 |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	// ���Ͽ��� ���̴��� ������
	D3DCompileFromFile(
		WPath.c_str(),         // ���̴� ���� ���
		nullptr,               // ��ũ�� ���� (����)
		nullptr,               // ���� ���� �ڵ鷯 (����)
		"VertexToWorld",       // ��Ʈ�� ����Ʈ �Լ� �̸�
		VSVersion.c_str(),     // Ÿ�� ���̴� ��
		Flag0,                 // ������ �÷��� 1
		Flag1,                 // ������ �÷��� 2
		&VSShaderCodeBlob,     // �����ϵ� ���̴� �ڵ� ���
		&VSErrorCodeBlob);     // ���� �ڵ� ���

	// �����ϵ� ���̴� �ڵ� ����� ���� ��� ���� ó��
	if (nullptr == VSShaderCodeBlob)
	{
		std::string ErrString = reinterpret_cast<char*>(VSErrorCodeBlob->GetBufferPointer());
		MSGASSERT("���ؽ� ���̴� ���� ���� Error code : " + ErrString);
		return;
	}

	// ���ؽ� ���̴� ����
	HRESULT Result = UEngineCore::GetDevice().GetDevice()->CreateVertexShader(
		VSShaderCodeBlob->GetBufferPointer(), // ���̴� �ڵ� ������
		VSShaderCodeBlob->GetBufferSize(),    // ���̴� �ڵ� ũ��
		nullptr,                              // Ŭ���� ��ũ (����)
		&VertexShader);                       // ������ ���̴� ��ü

	// ���̴� ���� ���� �� ���� ó��
	if (S_OK != Result)
	{
		MSGASSERT("���ؽ� ���̴� ���� ����");
		return;
	}

	// �Է� ����� ���̾ƿ� ����
	InitVertexLayout();

}

void UPrimitiveComponent::UpdateVertexShader()
{
	UEngineCore::GetDevice().GetDeviceContext()->VSSetShader(VertexShader.Get(), nullptr, 0);
}


void UPrimitiveComponent::UpdateIndexBuffer()
{
	int Offset = 0;
	Mesh->GetIndexBuffer()->Update();
	UEngineCore::GetDevice().GetDeviceContext()->IASetPrimitiveTopology(Topology);
}

void UPrimitiveComponent::InitRasterizer()
{
	// �����Ͷ����� ���� ���� ����ü �ʱ�ȭ
	D3D11_RASTERIZER_DESC Desc = {};

	// �ø� ��带 �� ���̽�(���� ���� ����)�� ����
	// �������� �޸��� ���������� �ʵ��� �Ͽ� ������ ���
	Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

	// ������ ä��� ��带 �ָ���� ����
	// �������� ���θ� ä��� �⺻ ������ ���
	Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	// ����̽��� ����Ͽ� �����Ͷ����� ���¸� ����
	// GPU�� �������� �������ϴ� ����� ����
	UEngineCore::GetDevice().GetDevice()->CreateRasterizerState(&Desc, &RasterizerState);

	// ����Ʈ ���� ����
	// ����Ʈ�� �������� �̹����� ȭ�鿡 �׷��� ������ �����մϴ�.
	ViewPortInfo.Width = UEngineCore::GetSceenScale().X;
	ViewPortInfo.Height = UEngineCore::GetSceenScale().Y;
	ViewPortInfo.TopLeftX = 0.0f; // ����Ʈ�� ���� X ��ǥ�� 0���� ���� (ȭ���� ���� �����ڸ�)
	ViewPortInfo.TopLeftY = 0.0f; // ����Ʈ�� ���� Y ��ǥ�� 0���� ���� (ȭ���� ��� �����ڸ�)
	ViewPortInfo.MinDepth = 0.0f; // ����Ʈ�� �ּ� ���̸� 0.0���� ���� (���� ����� ����)
	ViewPortInfo.MaxDepth = 1.0f; // ����Ʈ�� �ִ� ���̸� 1.0���� ���� (���� �� ����)


}

void UPrimitiveComponent::UpdateRasterizer()
{
	UEngineCore::GetDevice().GetDeviceContext()->RSSetViewports(1, &ViewPortInfo);
	UEngineCore::GetDevice().GetDeviceContext()->RSSetState(RasterizerState.Get());
}

void UPrimitiveComponent::InitPixelShader()
{
	// ���� ���丮 ���� ��ü ����
	FDirectoryHelper CurDir;
	// ���� ���̴� ���丮�� �̵�
	CurDir.MoveEngineShaderDirectory();
	// ���̴� ������ ������
	FFileHelper VSFile = CurDir.GetFile("DefaultShader.fx");
	// ���� ��θ� ���ڿ��� ��ȯ
	std::string Path = VSFile.GetPathToString();

	// ���ڿ� ��θ� ���̵� ���ڿ��� ��ȯ
	std::wstring WPath = UEngineString::AnsiToUnicode(Path);

	// ���̴� ������ ���� (�ȼ� ���̴� ���� 5.0)
	std::string PSVersion = "ps_5_0";

	// ������ �÷��� �ʱ�ȭ
	int Flag0 = 0;
	int Flag1 = 0;

	// ����� ��忡�� ����� �÷��� ����
#ifdef _DEBUG
	Flag0 = D3D10_SHADER_DEBUG;
#endif

	// ����� �� �켱 ������� ��ŷ�ϴ� �÷��� ����
	Flag0 |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	// ���Ͽ��� ���̴��� ������
	D3DCompileFromFile(
		WPath.c_str(),         // ���̴� ���� ���
		nullptr,               // ��ũ�� ���� (����)
		nullptr,               // ���� ���� �ڵ鷯 (����)
		"PixelToWorld",        // ��Ʈ�� ����Ʈ �Լ� �̸�
		PSVersion.c_str(),     // Ÿ�� ���̴� ��
		Flag0,                 // ������ �÷��� 1
		Flag1,                 // ������ �÷��� 2
		&PSShaderCodeBlob,     // �����ϵ� ���̴� �ڵ� ���
		&PSErrorCodeBlob);     // ���� �ڵ� ���

	// �����ϵ� ���̴� �ڵ� ����� ���� ��� ���� ó��
	if (nullptr == PSShaderCodeBlob)
	{
		std::string ErrString = reinterpret_cast<char*>(PSErrorCodeBlob->GetBufferPointer());
		MSGASSERT("���̴� �ڵ� �߰����忡�� �����߽��ϴ�\n" + ErrString);
		return;
	}

	// �ȼ� ���̴� ����
	HRESULT Result = UEngineCore::GetDevice().GetDevice()->CreatePixelShader(
		PSShaderCodeBlob->GetBufferPointer(), // ���̴� �ڵ� ������
		PSShaderCodeBlob->GetBufferSize(),    // ���̴� �ڵ� ũ��
		nullptr,                              // Ŭ���� ��ũ (����)
		&PixelShader);                        // ������ �ȼ� ���̴� ��ü

	// ���̴� ���� ���� �� ���� ó��
	if (S_OK != Result)
	{
		MSGASSERT("�ȼ� ���̴� ������ ����");
	}

}

void UPrimitiveComponent::UpdatePixelShader()
{
	UEngineCore::GetDevice().GetDeviceContext()->PSSetShader(PixelShader.Get(), nullptr, 0);

}

// �׸��� ���� ���� �غ� ����
void UPrimitiveComponent::UpdateRenderTargetView()
{
	if (nullptr != Blend)
	{
		Blend->Update();
	}

	// ���� Ÿ�� �� �����͸� ������
	ID3D11RenderTargetView* RenderTargetView = UEngineCore::GetDevice().GetRenderTargetView();

	// ���� Ÿ�� �� ������ �迭�� �ʱ�ȭ
	ID3D11RenderTargetView* ArrRtv[16] = { 0 }; // �ִ� 16���� ���� Ÿ�� �並 ���� �� ����
	ArrRtv[0] = RenderTargetView; // ù ��° ���� Ÿ�� �並 ���� (���⼭�� SV_Target0)

	// ��� ����(OM) ������������ ���� Ÿ���� ����
	UEngineCore::GetDevice().GetDeviceContext()->OMSetRenderTargets(
		1,            // ���� Ÿ�� ���� �� (���⼭�� 1��)
		&ArrRtv[0],   // ���� Ÿ�� �� �迭�� ������
		nullptr       // ����-���ٽ� �� (���⼭�� ��� �� ��)
	);

}

void UPrimitiveComponent::InitShaderResourceView()
{
	// ��� ���� ���� ����ü �ʱ�ȭ
	D3D11_BUFFER_DESC Desc = { 0 };
	Desc.ByteWidth = sizeof(FTransform); // ������ ũ�⸦ FTransform ����ü ũ��� ����
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // ������ ���ε� �÷��׸� ��� ���۷� ����
	Desc.CPUAccessFlags = 0; // CPU�� ���ۿ� ���� ���� �����ϵ��� ����
	Desc.Usage = D3D11_USAGE_DEFAULT; // ������ ��� ����� �������� ����

	// ����̽��� ����Ͽ� ��� ���� ����
	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&Desc, nullptr, &ConstantBuffer))
	{
		MSGASSERT("��� ���ۿ� ���� ����");
		return;
	}

	{
		D3D11_BUFFER_DESC Desc = { 0 };
		Desc.ByteWidth = sizeof(FUVValue);
		Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		Desc.Usage = D3D11_USAGE_DYNAMIC;

		if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&Desc, nullptr, &UVValue))
		{
			MSGASSERT("������� ������ �����߽��ϴ�..");
			return;
		}
	}
	
}

void UPrimitiveComponent::UpdateShaderResourceView()
{
	{
		FTransform& RendererTransform = GetComponentTransformRef();

		D3D11_MAPPED_SUBRESOURCE SubResourceData = {};

		////������ ���� �� ��� ���� ����
		//UEngineCore::GetDevice().GetDeviceContext()->Map(ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResourceData);

		//if (nullptr == SubResourceData.pData)
		//{
		//	MSGASSERT("�׷��� ī�� ���� �ź�");
		//}

		//memcpy_s(SubResourceData.pData, sizeof(FTransform), &RendererTransform, sizeof(FTransform));

		//UEngineCore::GetDevice().GetDeviceContext()->Unmap(ConstantBuffer.Get(), 0);

		//ID3D11Buffer* ArrPtr[16] = { ConstantBuffer.Get() };

		//UEngineCore::GetDevice().GetDeviceContext()->VSSetConstantBuffers(0, 1, ArrPtr);

		UEngineCore::GetDevice().GetDeviceContext()->UpdateSubresource(ConstantBuffer.Get(), 0, nullptr, &RendererTransform, 0, 0);

		ID3D11Buffer* arrPtr[16] = { ConstantBuffer.Get() };
		UEngineCore::GetDevice().GetDeviceContext()->VSSetConstantBuffers(0, 1, arrPtr);
	}

	{
		D3D11_MAPPED_SUBRESOURCE Data = {};
		// �� �����͸� ����ϴ� ������ ������ ��� ����
		// ��� �׷���ī��� �� �׷���ī�忡 �ִ� ������� �����ؾ� ��.
		UEngineCore::GetDevice().GetDeviceContext()->Map(UVValue.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);
		// Data.pData �׷���ī��� ����� �ּҰ�.
		if (nullptr == Data.pData)
		{
			MSGASSERT("�׷���ī�尡 ������ �ź��߽��ϴ�.");
		}
		memcpy_s(Data.pData, sizeof(FUVValue), &UVValueData, sizeof(FUVValue));
		UEngineCore::GetDevice().GetDeviceContext()->Unmap(UVValue.Get(), 0);
		// ���� ������۸� 
		ID3D11Buffer* ArrPtr[16] = { UVValue.Get() };
		UEngineCore::GetDevice().GetDeviceContext()->VSSetConstantBuffers(2, 1, ArrPtr);
	}
}

void UPrimitiveComponent::SetMesh(std::string_view MeshName)
{
	std::shared_ptr<UStaticMesh> FindMesh = UStaticMesh::Find<UStaticMesh>(MeshName);

	Mesh = FindMesh.get();
	
	if (nullptr == Mesh)
	{
		MSGASSERT("�������� �ʴ� �Ž��Դϴ�.");
	}
}

void UPrimitiveComponent::SetBlend(std::string_view BlendName)
{
	std::shared_ptr<UEngineBlend> FindBlend = UEngineBlend::Find<UEngineBlend>(BlendName);

	Blend = FindBlend.get();

	if (nullptr == Blend)
	{
		MSGASSERT("�������� �ʴ� ���� �Դϴ� UPrimitiveComponent::SetBlend\n");
	}
}

void UPrimitiveComponent::AddUVPlusValue(float4 Value)
{
}

void UPrimitiveComponent::SetTexture(UTexture* NewTexture)
{
	Texture = NewTexture;
}

void UPrimitiveComponent::SetTexture(std::string_view TextureName)
{
	Texture = UTexture::Find<UTexture>(TextureName).get();

	if (nullptr == Texture)
	{
		MSGASSERT("�������� �ʴ� �ؽ�ó�� �����Ϸ��� �߽��ϴ�");
	}
}

void UPrimitiveComponent::SetSpriteData(UPaperSprite* PaperSprite, size_t Index)
{
	SpriteData = PaperSprite->GetSpriteData(Index);
}

void UPrimitiveComponent::SetOrder(int NewOrder)
{
	int PrevOrder = GetOrder();
	UObject::SetOrder(NewOrder);
	ULevel* Level = GetOwner()->GetWorld();

	std::shared_ptr<UPrimitiveComponent> RendererPtr = GetThis<UPrimitiveComponent>();
	Level->ChangeRenderGroup(0, PrevOrder, RendererPtr);
}