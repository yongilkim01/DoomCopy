#include "pch.h"
#include "EngineConstantBuffer.h"

std::map<int, std::map<std::string, std::shared_ptr<UEngineConstantBuffer>>> UEngineConstantBuffer::BufferMap;


UEngineConstantBuffer::UEngineConstantBuffer()
{
}

UEngineConstantBuffer::~UEngineConstantBuffer()
{
}

std::shared_ptr<UEngineConstantBuffer> UEngineConstantBuffer::CreateOrFind(int _Byte, const std::string_view& _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	if (true == BufferMap.contains(_Byte))
	{
		if (true == BufferMap[_Byte].contains(UpperName))
		{
			return BufferMap[_Byte][UpperName];
		}
	}
	std::shared_ptr<UEngineConstantBuffer> NewRes = std::make_shared<UEngineConstantBuffer>();
	NewRes->AssetCreate();
	BufferMap[_Byte][UpperName] = NewRes;

	return nullptr;
}

void UEngineConstantBuffer::AssetCreate()
{
	/*
	{
		D3D11_BUFFER_DESC BufferInfo = { 0 };
		BufferInfo.ByteWidth = sizeof(FTransform);
		BufferInfo.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		BufferInfo.Usage = D3D11_USAGE_DYNAMIC;
		if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&BufferInfo, nullptr, &Buffer))
		{
			MSGASSERT("상수버퍼 생성에 실패했습니다..");
			return;
		}
	}
	*/
}

