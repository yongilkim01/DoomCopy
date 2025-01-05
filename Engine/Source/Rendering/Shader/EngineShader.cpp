#include "pch.h"
#include "EngineShader.h"

#include "Rendering/Shader/EngineVertexShader.h"
#include "Rendering/EngineConstantBuffer.h"
#include "Rendering/Shader/EnginePixelShader.h"
#include "Rendering/Sampler/EngineSampler.h"

#include "Classes/Engine/Texture.h"

UEngineShader::UEngineShader()
{
}

UEngineShader::~UEngineShader()
{
}

void UEngineShader::ReflectionCompile(FFileHelper& FileHelper)
{
	// 버텍스 셰이더와 픽셀 셰이더 초기화 및 생성

	FileHelper.FileOpen("rt");
	std::string ShaderCode = FileHelper.GetAllFileText();

	{
		size_t EntryIndex = ShaderCode.find("_VS(");
		if (EntryIndex != std::string::npos)
		{
			{
				// 역순으로 찾아나가는 함수
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
				// 역순으로 찾아나가는 함수
				size_t FirstIndex = ShaderCode.find_last_of(" ", EntryIndex);
				std::string EntryName = ShaderCode.substr(FirstIndex + 1, EntryIndex - FirstIndex - 1);
				EntryName += "_PS";

				UEnginePixelShader::Load(FileHelper.GetPathToString(), EntryName);
			}
		}
	}
}

void UEngineShader::ShaderResCheck()
{
	// 리플렉션이라는 용어는 c#등의 최신언어들에서 많이 등장하는데.
	// RTTI라고 보시면 됩니다.
	// RTTI 런타임 타입 인포메이션
	// C#으로 설명을 드리면 c#은 어떤 클래스 
	// classInfo Info = typeid(Player);
	// Info.FunctionCount();
	// Info.PrivateFunctionCount();
	// shader라면 어떨까?
	// Info.ConstantBufferCount();
	// 상수버퍼 및 모든 쉐이더에서 사용한 리소스 검색기능입니다.
	// 당연히 다이렉트 x에서 지원해주는 기능을 기반으로 

	if (nullptr == ShaderCodeBlob)
	{
		MSGASSERT("쉐이더가 컴파일되지 않아서 쉐이더의 리소스를 조사할수가 없습니다.");
		return;
	}

	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> CompileInfo = nullptr;

	// #include <d3dcompiler.h>
	if (S_OK != D3DReflect(ShaderCodeBlob->GetBufferPointer(), ShaderCodeBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &CompileInfo))
	{
		MSGASSERT("리플렉션에 실패했습니다.");
		return;
	}

	D3D11_SHADER_DESC Info;

	CompileInfo->GetDesc(&Info);

	D3D11_SHADER_INPUT_BIND_DESC ResDesc;

	// 상수버퍼 텍스처 가리지 않고 그냥 리소스 개수 Info.BoundResources
	for (UINT i = 0; i < Info.BoundResources; i++)
	{
		CompileInfo->GetResourceBindingDesc(i, &ResDesc);
		std::string Name = ResDesc.Name;
		std::string UpperName = UEngineString::ToUpper(Name);
		D3D_SHADER_INPUT_TYPE Type = ResDesc.Type;

		switch (Type)
		{
		case D3D_SIT_CBUFFER:
		{
			// 구체화된 정보를 얻어옵니다.
			ID3D11ShaderReflectionConstantBuffer* Info = CompileInfo->GetConstantBufferByName(ResDesc.Name);
			D3D11_SHADER_BUFFER_DESC BufferInfo = { 0 };
			Info->GetDesc(&BufferInfo);

			std::shared_ptr<UEngineConstantBuffer> Buffer = UEngineConstantBuffer::CreateOrFind(BufferInfo.Size, UpperName);

			UEngineConstantBufferRes NewRes;
			NewRes.ShaderType = ShaderType;
			NewRes.Name = UpperName;
			NewRes.BindIndex = ResDesc.BindPoint;
			NewRes.Res = Buffer;
			NewRes.BufferSize = BufferInfo.Size;

			ShaderResource.CreateConstantBufferRes(UpperName, NewRes);

			break;
		}
		case D3D_SIT_TEXTURE:
		{
			std::shared_ptr<UTexture> Res = UTexture::Find<UTexture>("NSBase.png");

			UEngineTextureRes NewRes;
			NewRes.ShaderType = ShaderType;
			NewRes.Name = UpperName;
			NewRes.BindIndex = ResDesc.BindPoint;
			NewRes.Res = Res;

			ShaderResource.CreateTextureRes(UpperName, NewRes);

			break;
		}
		case D3D_SIT_SAMPLER:
		{
			std::shared_ptr<UEngineSampler> Res = UEngineSampler::Find<UEngineSampler>("WRapSampler");
			UEngineSamplerRes NewRes;
			NewRes.ShaderType = ShaderType;
			NewRes.Name = UpperName;
			NewRes.BindIndex = ResDesc.BindPoint;
			NewRes.Res = Res;

			ShaderResource.CreateSamplerRes(UpperName, NewRes);

			break;
		}
		case D3D_SIT_STRUCTURED:
		{
			int a = 0;
			break;
		}
		case D3D_SIT_UAV_RWSTRUCTURED: // 컴퓨트
		{
			int a = 0;
			break;
		}
		default:
			break;
		}
		int a = 0;
	}

	EntryName;
}

