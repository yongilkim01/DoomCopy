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
		MSGASSERT("���� �̸� ������۰� �� ���̴��� 2���� �����մϴ�");
		return;
	}
	ConstantBufferRess[Name.data()] = Res;
}

