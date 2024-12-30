#include "pch.h"
#include "IndexBuffer.h"

FIndexBuffer::FIndexBuffer()
{
}

FIndexBuffer::~FIndexBuffer()
{
}

std::shared_ptr<FIndexBuffer> FIndexBuffer::Create(std::string_view Name, const void* InitData, size_t IndexSize, size_t IndexCount)
{
	std::string UpperName = ToUpperName(Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("�̹� �ε�� �����Դϴ�. " + UpperName);
		return nullptr;
	}

	std::shared_ptr<FIndexBuffer> NewRes = std::make_shared<FIndexBuffer>();

	AddAsset<FIndexBuffer>(NewRes, Name, "");

	NewRes->AssetCreate(InitData, IndexSize, IndexCount);

	return NewRes;
}

void FIndexBuffer::Update()
{
	int Offset = 0;
	UEngineCore::GetDevice().GetDeviceContext()->IASetIndexBuffer(IndexBuffer.Get(), Format, Offset);
}

void FIndexBuffer::AssetCreate(const void* InitData, size_t InIndexSize, size_t InIndexCount)
{
	IndexSize = static_cast<UINT>(InIndexSize);
	IndexCount = static_cast<UINT>(InIndexCount);

	if (4 == IndexSize) // 42��
	{
		Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
	}
	else if (2 == IndexSize) // 65536
	{
		Format = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
	}
	else
	{
		MSGASSERT("�������� �ʴ� �ε��� �����Դϴ�.");
	}

	IndexBufferDesc.ByteWidth = static_cast<UINT>(InIndexSize * InIndexCount);
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA Data; // �ʱⰪ �־��ִ� �뵵�� ����ü

	Data.pSysMem = InitData;

	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&IndexBufferDesc, &Data, &IndexBuffer))
	{
		MSGASSERT("�ε��� ���� ������ �����߽��ϴ�.");
		return;
	}
}

