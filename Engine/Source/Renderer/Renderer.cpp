#include "pch.h"
#include "Renderer.h"

#include "Classes/Engine/Level.h"
#include "Core/Misc/DirectoryHelper.h"
#include "Core/Misc/FileHelper.h"
#include "Core/Containers/EngineString.h"
#include "Core/EngineCore.h"
#include "Classes/Camera/CameraComponent.h"
#include "Core/Containers/EngineString.h"

#include "ThirdParty/DirectxTex/Include/DirectXTex.h"

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex_Release.lib")
#endif

/*
	������ ������ ����

	-------------------
	|   Vertex Data	  |
	-------------------
*/

URenderer::URenderer()
{
}

URenderer::~URenderer()
{
	VertexBuffer = nullptr;
	VSShaderCodeBlob = nullptr;
	VSErrorCodeBlob = nullptr;
}

void URenderer::BeginPlay()
{
	SetOrder(0);

	// ���ؽ� ���� ����
	InitVertexBuffer();

	// ���ؽ� ���̴� ����
	InitVertexShader();

	// �ε��� ���̴� ����
	InitIndexBuffer();


	InitRasterizer();
	InitPixelShader();
	InitShaderResourceView();
}

void URenderer::Render(UCameraComponent* CameraComponent, float DeltaTime)
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

	UEngineCore::Device.GetDeviceContext()->DrawIndexed(6, 0, 0);

}

void URenderer::InitVertexBuffer()
{
	// Vertex �����͸� ������ ���� ���� �� ũ�� ����
	std::vector<EngineVertex> Vertexes;
	Vertexes.resize(4);

	// �� Vertex�� ��ġ, �ؽ�ó ��ǥ �� ������ ����
	Vertexes[0] = EngineVertex{ FVector(-0.5f, 0.5f, -0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
	Vertexes[1] = EngineVertex{ FVector(0.5f, 0.5f, -0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} };
	Vertexes[2] = EngineVertex{ FVector(-0.5f, -0.5f, -0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} };
	Vertexes[3] = EngineVertex{ FVector(0.5f, -0.5f, -0.0f), {1.0f , 1.0f } , {1.0f, 1.0f, 1.0f, 1.0f} };

	// ���ؽ� ���� ���� ����ü �ʱ�ȭ
	D3D11_BUFFER_DESC Desc;
	ZeroMemory(&Desc, sizeof(D3D11_BUFFER_DESC));

	Desc.ByteWidth = sizeof(EngineVertex) * Vertexes.size(); // ������ ũ�⸦ ����
	Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				 // ������ ���ε� �÷��׸� ���ؽ� ���۷� ����
	Desc.CPUAccessFlags = 0;								 // CPU ���� �÷��׸� ���� (�⺻��)
	Desc.Usage = D3D11_USAGE_DEFAULT;						 // ������ ��� ����� ����

	// ���ۿ� �ʱ� ������ ����
	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = &Vertexes[0];

	// ����̽��� ����Ͽ� ���ؽ� ���۸� ����
	if (S_OK != UEngineCore::Device.GetDevice()->CreateBuffer(&Desc, &Data, VertexBuffer.GetAddressOf()))
	{
		MSGASSERT("���ؽ� ���� ���� ����");
		return;
	}


}

void URenderer::UpdateVertexBuffer()
{
	// ���ؽ��� ũ��� �������� ����
	UINT VertexSize = sizeof(EngineVertex); // ���ؽ� �ϳ��� ũ�⸦ ����
	UINT Offset = 0; // ���ؽ� �������� ���� �������� ���� (0���� ����)

	// ���ؽ� ���� �迭 ����
	ID3D11Buffer* ArrayBuffer[1];
	ArrayBuffer[0] = VertexBuffer.Get(); // ������ ���ؽ� ���۸� �迭�� ����

	// ���ؽ� ���۸� �Է� �����(IA)�� ����
	UEngineCore::Device.GetDeviceContext()->IASetVertexBuffers(
		0,            // �Է� ������ ���� �ε��� (���� 0���� ����)
		1,            // ������ ������ ���� (���⼭�� 1��)
		ArrayBuffer,  // ���� �迭�� ������
		&VertexSize,  // �� ���ؽ��� ũ�⸦ ����
		&Offset       // ���� ���� ������ ���� �������� ����
	);

	// �Է� ���̾ƿ��� �Է� �����(IA)�� ����
	UEngineCore::Device.GetDeviceContext()->IASetInputLayout(InputLayout.Get());

}

void URenderer::InitVertexLayout()
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
	HRESULT Result = UEngineCore::Device.GetDevice()->CreateInputLayout(
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

void URenderer::InitVertexShader()
{
	// ���� ���丮 ���� ��ü ����
	FDirectoryHelper CurDir;
	// ���� ���̴� ���丮�� �̵�
	CurDir.MoveEngineShaderDirectory();
	// ���̴� ������ ������
	FFileHelper VSFile = CurDir.GetFile("SpriteShader.fx");
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
	HRESULT Result = UEngineCore::Device.GetDevice()->CreateVertexShader(
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

void URenderer::UpdateVertexShader()
{
	UEngineCore::Device.GetDeviceContext()->VSSetShader(VertexShader.Get(), nullptr, 0);
}

void URenderer::InitIndexBuffer()
{
	// �ε��� �����͸� ������ ���� ����
	std::vector<unsigned int> Indexes;

	// �ε��� ������ �߰�
	Indexes.push_back(0);
	Indexes.push_back(1);
	Indexes.push_back(2);

	Indexes.push_back(1);
	Indexes.push_back(3);
	Indexes.push_back(2);

	// �ε��� ���� ���� ����ü �ʱ�ȭ
	D3D11_BUFFER_DESC Desc = { 0 };

	Desc.ByteWidth = sizeof(unsigned int) * static_cast<int>(Indexes.size()); // ������ ũ�⸦ ����
	Desc.BindFlags = D3D11_BIND_INDEX_BUFFER; // ������ ���ε� �÷��׸� ���� (�ε��� ���۷� ���)
	Desc.CPUAccessFlags = 0; // CPU ���� �÷��׸� ���� (�⺻��)
	Desc.Usage = D3D11_USAGE_DEFAULT; // ������ ��� ����� ����

	// ���ۿ� �ʱ� ������ ����
	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = &Indexes[0]; // �ʱ� ������ ����

	// ����̽��� ����Ͽ� �ε��� ���۸� ����
	if (S_OK != UEngineCore::Device.GetDevice()->CreateBuffer(&Desc, &Data, &IndexBuffer))
	{
		MSGASSERT("�ε��� ���� ���� ����");
		return;
	}

}

void URenderer::UpdateIndexBuffer()
{
	int Offset = 0;

	UEngineCore::Device.GetDeviceContext()->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, Offset);

	UEngineCore::Device.GetDeviceContext()->IASetPrimitiveTopology(Topology);
}

void URenderer::InitRasterizer()
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
	UEngineCore::Device.GetDevice()->CreateRasterizerState(&Desc, RasterizerState.GetAddressOf());

	// ����Ʈ ���� ����
	// ����Ʈ�� �������� �̹����� ȭ�鿡 �׷��� ������ �����մϴ�.
	ViewPortInfo.Width = 1280.0f; // ����Ʈ�� �ʺ� 1280���� ����
	ViewPortInfo.Height = 720.0f; // ����Ʈ�� ���̸� 720���� ����
	ViewPortInfo.TopLeftX = 0.0f; // ����Ʈ�� ���� X ��ǥ�� 0���� ���� (ȭ���� ���� �����ڸ�)
	ViewPortInfo.TopLeftY = 0.0f; // ����Ʈ�� ���� Y ��ǥ�� 0���� ���� (ȭ���� ��� �����ڸ�)
	ViewPortInfo.MinDepth = 0.0f; // ����Ʈ�� �ּ� ���̸� 0.0���� ���� (���� ����� ����)
	ViewPortInfo.MaxDepth = 1.0f; // ����Ʈ�� �ִ� ���̸� 1.0���� ���� (���� �� ����)


}

void URenderer::UpdateRasterizer()
{
	UEngineCore::Device.GetDeviceContext()->RSSetViewports(1, &ViewPortInfo);
	UEngineCore::Device.GetDeviceContext()->RSSetState(RasterizerState.Get());
}

void URenderer::InitPixelShader()
{
	// ���� ���丮 ���� ��ü ����
	FDirectoryHelper CurDir;
	// ���� ���̴� ���丮�� �̵�
	CurDir.MoveEngineShaderDirectory();
	// ���̴� ������ ������
	FFileHelper VSFile = CurDir.GetFile("SpriteShader.fx");
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
	HRESULT Result = UEngineCore::Device.GetDevice()->CreatePixelShader(
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

void URenderer::UpdatePixelShader()
{
	UEngineCore::Device.GetDeviceContext()->PSSetShader(PixelShader.Get(), nullptr, 0);

}

// �׸��� ���� ���� �غ� ����
void URenderer::UpdateRenderTargetView()
{
	// ���� Ÿ�� �� �����͸� ������
	ID3D11RenderTargetView* RenderTargetView = UEngineCore::Device.GetRenderTargetView();

	// ���� Ÿ�� �� ������ �迭�� �ʱ�ȭ
	ID3D11RenderTargetView* ArrRtv[16] = { 0 }; // �ִ� 16���� ���� Ÿ�� �並 ���� �� ����
	ArrRtv[0] = RenderTargetView; // ù ��° ���� Ÿ�� �並 ���� (���⼭�� SV_Target0)

	// ��� ����(OM) ������������ ���� Ÿ���� ����
	UEngineCore::Device.GetDeviceContext()->OMSetRenderTargets(
		1,            // ���� Ÿ�� ���� �� (���⼭�� 1��)
		&ArrRtv[0],   // ���� Ÿ�� �� �迭�� ������
		nullptr       // ����-���ٽ� �� (���⼭�� ��� �� ��)
	);

}

void URenderer::InitShaderResourceView()
{
	// ��� ���� ����
	{
		// ��� ���� ���� ����ü �ʱ�ȭ
		D3D11_BUFFER_DESC Desc = { 0 };
		Desc.ByteWidth = sizeof(FTransform); // ������ ũ�⸦ FTransform ����ü ũ��� ����
		Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // ������ ���ε� �÷��׸� ��� ���۷� ����
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE; // CPU�� ���ۿ� ���� ���� �����ϵ��� ����
		Desc.Usage = D3D11_USAGE_DYNAMIC; // ������ ��� ����� �������� ����

		// ����̽��� ����Ͽ� ��� ���� ����
		if (S_OK != UEngineCore::Device.GetDevice()->CreateBuffer(&Desc, nullptr, &ConstantBuffer))
		{
			MSGASSERT("��� ���ۿ� ���� ����");
			return;
		}
	}

	// ���� ���丮 ���� ��ü ����
	FDirectoryHelper CurDirectory;
	// �θ� ���丮�� �̵��Ͽ� "Resources" ���丮 ����
	CurDirectory.MoveParentToDirectory("Resources");
	CurDirectory.MoveParentToDirectory("TitleLevel");
	// "Player.png" ������ ������
	FFileHelper FileHelper = CurDirectory.GetFile("TitleLogo.png");

	// ���� ��θ� ���ڿ��� ��ȯ
	std::string Str = FileHelper.GetPathToString();
	// ���� Ȯ���ڸ� ������
	std::string Ext = FileHelper.GetExtension();

	// ���ڿ� ��θ� ���̵� ���ڿ��� ��ȯ
	std::wstring WLoadPath = UEngineString::AnsiToUnicode(Str.c_str());
	// Ȯ���ڸ� �빮�ڷ� ��ȯ
	std::string UpperExt = UEngineString::ToUpper(Ext.c_str());

	DirectX::TexMetadata Metadata; // �ؽ�ó ��Ÿ������ ��ü ����
	DirectX::ScratchImage ImageData; // �ؽ�ó �̹��� �����͸� ������ ��ü ����

	// ���� Ȯ���ڿ� ���� ������ �Լ��� ���� �ε�
	if (UpperExt == ".DDS")
	{
		// DDS ���� �ε�
		if (S_OK != DirectX::LoadFromDDSFile(WLoadPath.c_str(), DirectX::DDS_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT("DDS ���� �ε忡 �����߽��ϴ�.");
			return;
		}
	}
	else if (UpperExt == ".TGA")
	{
		// TGA ���� �ε�
		if (S_OK != DirectX::LoadFromTGAFile(WLoadPath.c_str(), DirectX::TGA_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT("TGA ���� �ε忡 �����߽��ϴ�.");
			return;
		}
	}
	else
	{
		// �� ���� ���� ����(WIC ���)
		if (S_OK != DirectX::LoadFromWICFile(WLoadPath.c_str(), DirectX::WIC_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT(UpperExt + "���� �ε忡 �����߽��ϴ�.");
			return;
		}
	}

	// ���̴� ���ҽ� �� ����
	if (S_OK != DirectX::CreateShaderResourceView(
		UEngineCore::Device.GetDevice(),	// ����̽� ��ü
		ImageData.GetImages(),				// �̹��� ������ �迭
		ImageData.GetImageCount(),			// �̹��� ����
		ImageData.GetMetadata(),			// �̹��� ��Ÿ������
		ShaderResourceView.GetAddressOf()   // ������ ���̴� ���ҽ� ��
	))
	{
		MSGASSERT(UpperExt + "���̴� ���ҽ� �� ������ �����߽��ϴ�.");
		return;
	}

	// ���÷� ���� ���� ����ü �ʱ�ȭ �� ����
	D3D11_SAMPLER_DESC SamplerDesc = { D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT };
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER; // U ��ǥ �ؽ�ó ���� ��� ����
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER; // V ��ǥ �ؽ�ó ���� ��� ����
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP; // W ��ǥ �ؽ�ó ���� ��� ����

	SamplerDesc.BorderColor[0] = 0.0f;
	SamplerDesc.BorderColor[1] = 0.0f;
	SamplerDesc.BorderColor[2] = 0.0f;
	SamplerDesc.BorderColor[3] = 0.0f;

	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	//SamplerDesc.MaxLOD = 0.0f;
	//SamplerDesc.MinLOD = 0.0f;


	// ����̽��� ����Ͽ� ���÷� ���� ����
	UEngineCore::Device.GetDevice()->CreateSamplerState(&SamplerDesc, &SamplerState);

}

void URenderer::UpdateShaderResourceView()
{
	FTransform& RendererTransform = GetComponentTransformRef();

	D3D11_MAPPED_SUBRESOURCE MappedSubResource = {};

	UEngineCore::Device.GetDeviceContext()->Map(ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubResource);

	if (nullptr == MappedSubResource.pData)
	{
		MSGASSERT("�׷��� ī�� ���� ���� �ź�");
	}

	memcpy_s(MappedSubResource.pData, sizeof(FTransform), &RendererTransform, sizeof(FTransform));

	UEngineCore::Device.GetDeviceContext()->Unmap(ConstantBuffer.Get(), 0);

	ID3D11Buffer* ArrPtr[16] = { ConstantBuffer.Get() };
	UEngineCore::Device.GetDeviceContext()->VSSetConstantBuffers(0, 1, ArrPtr);

	ID3D11ShaderResourceView* ArrSRV[16] = { ShaderResourceView.Get() };
	UEngineCore::Device.GetDeviceContext()->PSSetShaderResources(0, 1, ArrSRV);

	ID3D11SamplerState* ArrSMP[16] = { SamplerState.Get() };
	UEngineCore::Device.GetDeviceContext()->PSSetSamplers(0, 1, ArrSMP);
}

void URenderer::SetOrder(int NewOrder)
{
	int PrevOrder = GetOrder();
	UObject::SetOrder(NewOrder);
	ULevel* Level = GetOwner()->GetWorld();

	std::shared_ptr<URenderer> RendererPtr = GetThis<URenderer>();
	Level->ChangeRenderGroup(0, PrevOrder, RendererPtr);
}