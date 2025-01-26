#include "pch.h"
#include "Rendering/Public/Buffer/IndexBuffer.h"

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
		MSGASSERT("이미 로드된 에셋입니다. " + UpperName);
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
	UGameEngine::GetDeviceContext()->IASetIndexBuffer(Buffer.Get(), Format, Offset);
}

void FIndexBuffer::AssetCreate(const void* InitData, size_t InIndexSize, size_t InIndexCount)
{
	IndexSize = static_cast<UINT>(InIndexSize);
	IndexCount = static_cast<UINT>(InIndexCount);

	if (4 == IndexSize) // 42억
	{
		Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
	}
	else if (2 == IndexSize) // 65536
	{
		Format = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
	}
	else
	{
		MSGASSERT("지원하지 않는 인덱스 포맷입니다.");
	}

	BufferInfo.ByteWidth = static_cast<UINT>(InIndexSize * InIndexCount);
	BufferInfo.BindFlags = D3D11_BIND_INDEX_BUFFER;
	BufferInfo.CPUAccessFlags = 0;
	BufferInfo.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA Data; // 초기값 넣어주는 용도의 구조체

	Data.pSysMem = InitData;

	if (S_OK != UGameEngine::GetDevice()->CreateBuffer(&BufferInfo, &Data, &Buffer))
	{
		MSGASSERT("인덱스 버퍼 생성에 실패했습니다.");
		return;
	}
}

