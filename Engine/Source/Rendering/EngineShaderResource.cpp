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
	if (true == ConstantBufferRes.contains(Name.data()))
	{
		MSGASSERT("���� �̸� ������۰� �� ���̴��� 2���� �����մϴ�");
		return;
	}
	ConstantBufferRes[Name.data()] = Res;
}

void UEngineShaderResource::Update()
{
	for (std::pair<const std::string, UEngineConstantBufferRes>& Res : ConstantBufferRes)
	{
		Res.second.Update();
	}
}

