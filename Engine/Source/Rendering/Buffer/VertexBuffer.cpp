#include "pch.h"
#include "VertexBuffer.h"

#include "Rendering/Buffer/EngineInputLayoutInfo.h"

FVertexBuffer::FVertexBuffer()
{
}

FVertexBuffer::~FVertexBuffer()
{
}

std::shared_ptr<FVertexBuffer> FVertexBuffer::Create(std::string_view Name, const void* InitData, size_t VertexSize, size_t VertexCount, UEngineInputLayoutInfo* InfoPtr)
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
	NewRes->InfoPtr = InfoPtr;

	return NewRes;
}

void FVertexBuffer::Update()
{
	UINT Offset = 0;
	ID3D11Buffer* ArrBuffer[1];
	ArrBuffer[0] = Buffer.Get();
	UEngineCore::GetDevice().GetDeviceContext()->IASetVertexBuffers(0, 1, ArrBuffer, &VertexSize, &Offset);
}

void FVertexBuffer::AssetCreate(const void* InitData, size_t InVertexSize, size_t InVertexCount)
{
	VertexCount = static_cast<UINT>(InVertexCount);
	VertexSize = static_cast<UINT>(InVertexSize);

	BufferInfo.ByteWidth = static_cast<UINT>(InVertexSize * InVertexCount);
	BufferInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferInfo.CPUAccessFlags = 0;
	BufferInfo.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA Data; // �ʱⰪ �־��ִ� �뵵�� ����ü

	Data.pSysMem = InitData;

	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&BufferInfo, &Data, &Buffer))
	{
		MSGASSERT("���ؽ� ���� ������ �����߽��ϴ�.");
		return;
	}
}

