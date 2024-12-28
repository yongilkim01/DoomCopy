#include "pch.h"
#include "LineComponent.h"

#include <Classes/Camera/CameraComponent.h>
#include <Core/Misc/DirectoryHelper.h>
#include <Core/Misc/FileHelper.h>

ULineComponent::ULineComponent()
{
}

ULineComponent::~ULineComponent()
{
	VertexBuffer = nullptr;
	VSShaderCodeBlob = nullptr;
	VSErrorCodeBlob = nullptr;
}

void ULineComponent::BeginPlay()
{
	USceneComponent::BeginPlay();

	SetOrder(0);

	InitVertexBuffer();
	InitVertexShader();
	InitIndexBuffer();
	InitRasterizer();
	InitPixelShader();
	InitShaderResourceView();
}

void ULineComponent::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	FTransform& CameraTransform = CameraComponent->GetComponentTransformRef();
	FTransform& RendererTransform = GetComponentTransformRef();

	RendererTransform.View = CameraTransform.View;
	RendererTransform.Projection = CameraTransform.Projection;

	RendererTransform.WVP = RendererTransform.World * RendererTransform.View * RendererTransform.Projection;

	// Rendering pipeline
	UpdateShaderResourceView();
	UpdateVertexBuffer();
	UpdateVertexShader();
	UpdateIndexBuffer();
	UpdateRasterizer();
	UpdatePixelShader();
	UpdateRenderTargetView();

	UEngineCore::GetDevice().GetDeviceContext()->DrawIndexed(2, 0, 0);
}

void ULineComponent::InitVertexBuffer()
{
	// Vertex �����͸� ������ ���� ���� �� ũ�� ����
	std::vector<LineVertex> Vertexes;
	Vertexes.resize(4);

	// �� Vertex�� ��ġ, �ؽ�ó ��ǥ �� ������ ����
	Vertexes[0] = LineVertex{ FVector(-0.5f, 0.0f, -0.0f), {1.0f, 0.0f, 0.0f, 1.0f} };
	Vertexes[1] = LineVertex{ FVector(0.5f, 0.0f, -0.0f), {0.0f, 1.0f, 0.0f, 1.0f} };

	// ���ؽ� ���� ���� ����ü �ʱ�ȭ
	D3D11_BUFFER_DESC Desc = { 0 };

	Desc.ByteWidth = static_cast <UINT>(sizeof(LineVertex) * Vertexes.size()); // ������ ũ�⸦ ����
	Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				 // ������ ���ε� �÷��׸� ���ؽ� ���۷� ����
	Desc.CPUAccessFlags = 0;								 // CPU ���� �÷��׸� ���� (�⺻��)
	Desc.Usage = D3D11_USAGE_DEFAULT;						 // ������ ��� ����� ����

	// ���ۿ� �ʱ� ������ ����
	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = &Vertexes[0];

	// ����̽��� ����Ͽ� ���ؽ� ���۸� ����
	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&Desc, &Data, VertexBuffer.GetAddressOf()))
	{
		MSGASSERT("���ؽ� ���� ���� ����");
		return;
	}


}

void ULineComponent::UpdateVertexBuffer()
{
	// ���ؽ��� ũ��� �������� ����
	UINT VertexSize = sizeof(LineVertex); // ���ؽ� �ϳ��� ũ�⸦ ����
	UINT Offset = 0; // ���ؽ� �������� ���� �������� ���� (0���� ����)

	// ���ؽ� ���� �迭 ����
	ID3D11Buffer* ArrayBuffer[1];
	ArrayBuffer[0] = VertexBuffer.Get(); // ������ ���ؽ� ���۸� �迭�� ����

	// ���ؽ� ���۸� �Է� �����(IA)�� ����
	UEngineCore::GetDevice().GetDeviceContext()->IASetVertexBuffers(
		0,            // �Է� ������ ���� �ε��� (���� 0���� ����)
		1,            // ������ ������ ���� (���⼭�� 1��)
		ArrayBuffer,  // ���� �迭�� ������
		&VertexSize,  // �� ���ؽ��� ũ�⸦ ����
		&Offset       // ���� ���� ������ ���� �������� ����
	);

	// �Է� ���̾ƿ��� �Է� �����(IA)�� ����
	UEngineCore::GetDevice().GetDeviceContext()->IASetInputLayout(InputLayout.Get());

}

void ULineComponent::InitVertexLayout()
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

	// COLOR �Է� ��� ���� �߰�
	{
		D3D11_INPUT_ELEMENT_DESC Desc;
		Desc.SemanticName = "COLOR"; // �ǹ̷� �̸�
		Desc.InputSlot = 0; // �Է� ����
		Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // ������ ����
		Desc.AlignedByteOffset = 16; // �������� ����Ʈ ������
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

void ULineComponent::InitVertexShader()
{
	// ���� ���丮 ���� ��ü ����
	FDirectoryHelper CurDir;
	// ���� ���̴� ���丮�� �̵�
	CurDir.MoveSelectShaderDirectory("Doom");
	// ���̴� ������ ������
	FFileHelper VSFile = CurDir.GetFile("DoomMapShader.fx");
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
		"VS",       // ��Ʈ�� ����Ʈ �Լ� �̸�
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

void ULineComponent::UpdateVertexShader()
{
	UEngineCore::GetDevice().GetDeviceContext()->VSSetShader(VertexShader.Get(), nullptr, 0);
}

void ULineComponent::InitIndexBuffer()
{
	// �ε��� �����͸� ������ ���� ����
	std::vector<unsigned int> Indexes;

	// �ε��� ������ �߰�
	Indexes.push_back(0);
	Indexes.push_back(1);

	// �ε��� ���� ���� ����ü �ʱ�ȭ
	D3D11_BUFFER_DESC Desc = { 0 };

	Desc.ByteWidth = static_cast <UINT>(sizeof(unsigned int) * static_cast<int>(Indexes.size())); // ������ ũ�⸦ ����
	Desc.BindFlags = D3D11_BIND_INDEX_BUFFER; // ������ ���ε� �÷��׸� ���� (�ε��� ���۷� ���)
	Desc.CPUAccessFlags = 0; // CPU ���� �÷��׸� ���� (�⺻��)
	Desc.Usage = D3D11_USAGE_DEFAULT; // ������ ��� ����� ����

	// ���ۿ� �ʱ� ������ ����
	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = &Indexes[0]; // �ʱ� ������ ����

	// ����̽��� ����Ͽ� �ε��� ���۸� ����
	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&Desc, &Data, &IndexBuffer))
	{
		MSGASSERT("�ε��� ���� ���� ����");
		return;
	}

}

void ULineComponent::UpdateIndexBuffer()
{
	int Offset = 0;

	UEngineCore::GetDevice().GetDeviceContext()->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, Offset);

	UEngineCore::GetDevice().GetDeviceContext()->IASetPrimitiveTopology(Topology);
}

void ULineComponent::InitRasterizer()
{
	// �����Ͷ����� ���� ���� ����ü �ʱ�ȭ
	D3D11_RASTERIZER_DESC Desc = {};

	// �ø� ��带 �� ���̽�(���� ���� ����)�� ����
	// �������� �޸��� ���������� �ʵ��� �Ͽ� ������ ���
	Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	// ������ ä��� ��带 �ָ���� ����
	// �������� ���θ� ä��� �⺻ ������ ���
	Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	// ����̽��� ����Ͽ� �����Ͷ����� ���¸� ����
	// GPU�� �������� �������ϴ� ����� ����
	UEngineCore::GetDevice().GetDevice()->CreateRasterizerState(&Desc, RasterizerState.GetAddressOf());

	// ����Ʈ ���� ����
	// ����Ʈ�� �������� �̹����� ȭ�鿡 �׷��� ������ �����մϴ�.
	ViewPortInfo.Width = 1280.0f; // ����Ʈ�� �ʺ� 1280���� ����
	ViewPortInfo.Height = 720.0f; // ����Ʈ�� ���̸� 720���� ����
	ViewPortInfo.TopLeftX = 0.0f; // ����Ʈ�� ���� X ��ǥ�� 0���� ���� (ȭ���� ���� �����ڸ�)
	ViewPortInfo.TopLeftY = 0.0f; // ����Ʈ�� ���� Y ��ǥ�� 0���� ���� (ȭ���� ��� �����ڸ�)
	ViewPortInfo.MinDepth = 0.0f; // ����Ʈ�� �ּ� ���̸� 0.0���� ���� (���� ����� ����)
	ViewPortInfo.MaxDepth = 1.0f; // ����Ʈ�� �ִ� ���̸� 1.0���� ���� (���� �� ����)


}

void ULineComponent::UpdateRasterizer()
{
	UEngineCore::GetDevice().GetDeviceContext()->RSSetViewports(1, &ViewPortInfo);
	UEngineCore::GetDevice().GetDeviceContext()->RSSetState(RasterizerState.Get());
}

void ULineComponent::InitPixelShader()
{
	// ���� ���丮 ���� ��ü ����
	FDirectoryHelper CurDir;
	// ���� ���̴� ���丮�� �̵�
	CurDir.MoveSelectShaderDirectory("Doom");
	// ���̴� ������ ������
	FFileHelper VSFile = CurDir.GetFile("DoomMapShader.fx");
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
		"PS",        // ��Ʈ�� ����Ʈ �Լ� �̸�
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

void ULineComponent::UpdatePixelShader()
{
	UEngineCore::GetDevice().GetDeviceContext()->PSSetShader(PixelShader.Get(), nullptr, 0);

}

// �׸��� ���� ���� �غ� ����
void ULineComponent::UpdateRenderTargetView()
{
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

void ULineComponent::InitShaderResourceView()
{
	D3D11_BUFFER_DESC BufferInfo = { 0 };
	BufferInfo.ByteWidth = sizeof(FTransform);
	BufferInfo.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	BufferInfo.Usage = D3D11_USAGE_DYNAMIC;

	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&BufferInfo, nullptr, ConstantBuffer.GetAddressOf()))
	{
		MSGASSERT("������� ������ �����߽��ϴ�..");
		return;
	}
}

void ULineComponent::UpdateShaderResourceView()
{
	{
		FTransform& RendererTransform = GetComponentTransformRef();

		D3D11_MAPPED_SUBRESOURCE SubResourceData = {};

		//������ ���� �� ��� ���� ����
		UEngineCore::GetDevice().GetDeviceContext()->Map(ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResourceData);

		if (nullptr == SubResourceData.pData)
		{
			MSGASSERT("�׷��� ī�� ���� �ź�");
		}

		memcpy_s(SubResourceData.pData, sizeof(FTransform), &RendererTransform, sizeof(FTransform));
		UEngineCore::GetDevice().GetDeviceContext()->Unmap(ConstantBuffer.Get(), 0);

		ID3D11Buffer* ArrPtr[16] = { ConstantBuffer.Get() };
		UEngineCore::GetDevice().GetDeviceContext()->VSSetConstantBuffers(0, 1, ArrPtr);
	}
}