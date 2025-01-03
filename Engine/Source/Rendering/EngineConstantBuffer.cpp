#include "pch.h"
#include "EngineConstantBuffer.h"

std::map<int, std::map<std::string, std::shared_ptr<UEngineConstantBuffer>>> UEngineConstantBuffer::BufferMap;


UEngineConstantBuffer::UEngineConstantBuffer()
{
}

UEngineConstantBuffer::~UEngineConstantBuffer()
{
}

std::shared_ptr<UEngineConstantBuffer> UEngineConstantBuffer::CreateOrFind(UINT _Byte, const std::string_view& _Name)
{
	if (0 == _Byte)
	{
		MSGASSERT("0����Ʈ ������۰� ����������� �߽��ϴ�.");
	}

	std::string UpperName = UEngineString::ToUpper(_Name);
	if (true == BufferMap.contains(_Byte))
	{
		if (true == BufferMap[_Byte].contains(UpperName))
		{
			return BufferMap[_Byte][UpperName];
		}
	}
	std::shared_ptr<UEngineConstantBuffer> NewRes = std::make_shared<UEngineConstantBuffer>();
	NewRes->SetName(UpperName);
	NewRes->AssetCreate(_Byte);
	BufferMap[_Byte][UpperName] = NewRes;

	return NewRes;
}

void UEngineConstantBuffer::ChangeData(void* _Data, UINT _Size)
{
	if (_Size != BufferInfo.ByteWidth)
	{
		MSGASSERT("����Ʈ ũ�Ⱑ �ٸ��� ���õǾ����ϴ�" + GetName());
		return;
	}
	// FTransform& RendererTrans = GetTransformRef();
	D3D11_MAPPED_SUBRESOURCE Data = {};
	// �� �����͸� ����ϴ� ������ ������ ��� ����
	// ��� �׷���ī��� �� �׷���ī�忡 �ִ� ������� �����ؾ� ��.
	UEngineCore::GetDevice().GetDeviceContext()->Map(Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);
	// Data.pData �׷���ī��� ����� �ּҰ�.
	if (nullptr == Data.pData)
	{
		MSGASSERT("�׷���ī�尡 ������ �ź��߽��ϴ�.");
	}
	memcpy_s(Data.pData, sizeof(FTransform), _Data, sizeof(FTransform));
	UEngineCore::GetDevice().GetDeviceContext()->Unmap(Buffer.Get(), 0);
}

void UEngineConstantBuffer::Update(EShaderType Type, UINT BindIndex)
{
	ID3D11Buffer* ArrPtr[1] = { Buffer.Get() };
	switch (Type)
	{
	case EShaderType::VS:
		UEngineCore::GetDevice().GetDeviceContext()->VSSetConstantBuffers(BindIndex, 1, ArrPtr);
		break;
	case EShaderType::PS:
		UEngineCore::GetDevice().GetDeviceContext()->PSSetConstantBuffers(BindIndex, 1, ArrPtr);
		break;
	case EShaderType::HS:
	case EShaderType::DS:
	case EShaderType::GS:
	case EShaderType::CS:
	default:
		MSGASSERT("���� �������� �ʴ� ���̴��� �����Ϸ��� �߽��ϴ�.");
		break;
	}
}

void UEngineConstantBuffer::AssetCreate(UINT Byte)
{
	{
		D3D11_BUFFER_DESC BufferInfo = { 0 };
		BufferInfo.ByteWidth = Byte;
		BufferInfo.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		BufferInfo.Usage = D3D11_USAGE_DYNAMIC;

		if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&BufferInfo, nullptr, &Buffer))
		{
			MSGASSERT("������� ������ �����߽��ϴ�");
			return;
		}
	}
}

