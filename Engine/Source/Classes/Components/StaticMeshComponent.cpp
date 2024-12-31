#include "pch.h"
#include "StaticMeshComponent.h"

#include "Core/Misc/DirectoryHelper.h"
#include "Core/Misc/FileHelper.h"

UStaticMeshComponent::UStaticMeshComponent()
{
}

UStaticMeshComponent::~UStaticMeshComponent()
{
}

void UStaticMeshComponent::BeginPlay()
{
	UPrimitiveComponent::BeginPlay();
	SetMesh("Map5");
}

void UStaticMeshComponent::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	UPrimitiveComponent::Render(CameraComponent, DeltaTime);
}


void UStaticMeshComponent::Init(std::string_view NewObjPath, std::string_view NewMtlPath)
{
	ObjPath = NewObjPath;
	MtlPath = NewMtlPath;
}

void UStaticMeshComponent::InitShaderResourceView()
{
	UPrimitiveComponent::InitShaderResourceView();

	D3D11_SAMPLER_DESC SamplerDesc;
	ZeroMemory(&SamplerDesc, sizeof(SamplerDesc));
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT HR = UEngineCore::GetDevice().GetDevice()->CreateSamplerState(&SamplerDesc, &SamplerState);

	if (FAILED(HR))
	{
		MSGASSERT("Texture sampler state couldn't be created");
	}
}

void UStaticMeshComponent::UpdateShaderResourceView()
{
	FTransform& RendererTrans = GetComponentTransformRef();

	UEngineCore::GetDevice().GetDeviceContext()->UpdateSubresource(ConstantBuffer.Get(), 0, nullptr, &RendererTrans, 0, 0);

	ID3D11Buffer* arrPtr[16] = { ConstantBuffer.Get() };
	UEngineCore::GetDevice().GetDeviceContext()->VSSetConstantBuffers(0, 1, arrPtr);

	ID3D11SamplerState* ArrSMP[16] = { SamplerState.Get() };
	UEngineCore::GetDevice().GetDeviceContext()->PSSetSamplers(0, 1, ArrSMP);
}

void UStaticMeshComponent::InitVertexShader()
{
	// ���� ���丮 ���� ��ü ����
	FDirectoryHelper CurDir;
	// ���� ���̴� ���丮�� �̵�
	CurDir.MoveEngineShaderDirectory();
	// ���̴� ������ ������
	FFileHelper VSFile = CurDir.GetFile("MeshShader.fx");
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

void UStaticMeshComponent::InitPixelShader()
{
	// ���� ���丮 ���� ��ü ����
	FDirectoryHelper CurDir;
	// ���� ���̴� ���丮�� �̵�
	CurDir.MoveEngineShaderDirectory();
	// ���̴� ������ ������
	FFileHelper VSFile = CurDir.GetFile("MeshShader.fx");
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