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
}

void FIndexBuffer::AssetCreate(const void* InitData, size_t IndexSize, size_t IndexCount)
{
	IndexBufferDesc.ByteWidth = static_cast<UINT>(IndexSize * IndexCount);
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA Data; // �ʱⰪ �־��ִ� �뵵�� ����ü

	Data.pSysMem = InitData;

	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&IndexBufferDesc, &Data, IndexBuffer.GetAddressOf()))
	{
		MSGASSERT("�ε��� ���� ������ �����߽��ϴ�.");
		return;
	}
}

