#include "pch.h"
#include "VertexBuffer.h"

FVertexBuffer::FVertexBuffer()
{
}

FVertexBuffer::~FVertexBuffer()
{
}

std::shared_ptr<FVertexBuffer> FVertexBuffer::Create(std::string_view Name, const void* InitData, size_t VertexSize, size_t VertexCount)
{
    std::string UpperName = ToUpperName(Name);

    if (true == Contains(UpperName))
    {
        MSGASSERT("�̹� �ε�� �����Դϴ�. " + UpperName);
        return nullptr;
    }

    std::shared_ptr<FVertexBuffer> NewRes = std::make_shared<FVertexBuffer>();

    AddAsset<FVertexBuffer>(NewRes, Name, "");

    NewRes->AssetCreate(InitData, VertexSize, VertexCount);
}

void FVertexBuffer::AssetCreate(const void* InitData, size_t VertexSize, size_t VertexCount)
{
	VertexBufferDesc.ByteWidth = static_cast<UINT>(VertexSize * VertexCount);
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA Data; // �ʱⰪ �־��ִ� �뵵�� ����ü

	Data.pSysMem = InitData;

	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&VertexBufferDesc, &Data, VertexBuffer.GetAddressOf()))
	{
		MSGASSERT("���ؽ� ���� ������ �����߽��ϴ�.");
		return;
	}
}

