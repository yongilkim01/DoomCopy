#include "pch.h"
#include "EngineShader.h"

#include "EngineVertexShader.h"

UEngineShader::UEngineShader()
{
}

UEngineShader::~UEngineShader()
{
}

void UEngineShader::ReflectionCompile(FFileHelper& FileHelper)
{
	// TODO: ���ؽ� ���̴��� �ȼ� ���̴� �����
	FileHelper.FileOpen("rt");
	std::string ShaderCode = FileHelper.GetAllFileText();

	{
		size_t EntryIndex = ShaderCode.find("_VS(");
		if (EntryIndex != std::string::npos)
		{
			{
				// �������� ã�Ƴ����� �Լ�
				size_t FirstIndex = ShaderCode.find_last_of(" ", EntryIndex);
				std::string EntryName = ShaderCode.substr(FirstIndex + 1, EntryIndex - FirstIndex - 1);
				EntryName += "_VS";
				UEngineVertexShader::Load(FileHelper.GetPathToString(), EntryName);

			}
		}
	}
	{
		size_t EntryIndex = ShaderCode.find("_PS(");
		if (EntryIndex != std::string::npos)
		{
			{
				// �������� ã�Ƴ����� �Լ�
				size_t FirstIndex = ShaderCode.find_last_of(" ", EntryIndex);
				std::string EntryName = ShaderCode.substr(FirstIndex + 1, EntryIndex - FirstIndex - 1);
				EntryName += "_PS";
				int a = 0;
			}
		}
	}
}

void UEngineShader::ShaderResCheck()
{
	// ���÷����̶�� ���� c#���� �ֽž��鿡�� ���� �����ϴµ�.
	// RTTI��� ���ø� �˴ϴ�.
	// RTTI ��Ÿ�� Ÿ�� �������̼�
	// C#���� ������ �帮�� c#�� � Ŭ���� 
	// classInfo Info = typeid(Player);
	// Info.FunctionCount();
	// Info.PrivateFunctionCount();
	// shader��� ���?
	// Info.ConstantBufferCount();
	// ������� �� ��� ���̴����� ����� ���ҽ� �˻�����Դϴ�.
	// �翬�� ���̷�Ʈ x���� �������ִ� ����� ������� 

	if (nullptr == ShaderCodeBlob)
	{
		MSGASSERT("���̴��� �����ϵ��� �ʾƼ� ���̴��� ���ҽ��� �����Ҽ��� �����ϴ�.");
		return;
	}

	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> CompileInfo = nullptr;

	// #include <d3dcompiler.h>
	if (S_OK != D3DReflect(ShaderCodeBlob->GetBufferPointer(), ShaderCodeBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &CompileInfo))
	{
		MSGASSERT("���÷��ǿ� �����߽��ϴ�.");
		return;
	}

	D3D11_SHADER_DESC Info;
}

