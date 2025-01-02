#include "pch.h"
#include "EngineShaderResource.h"

UEngineShaderResource::UEngineShaderResource()
{
}

UEngineShaderResource::~UEngineShaderResource()
{
}

void UEngineShaderResource::CreateConstantBufferRes(std::string_view Name, UEngineConstantBufferRes Res)
{
	if (true == ConstantBufferRess.contains(Name.data()))
	{
		MSGASSERT("같은 이름 상수버퍼가 한 쉐이더에 2개가 존재합니다");
		return;
	}
	ConstantBufferRess[Name.data()] = Res;
}

