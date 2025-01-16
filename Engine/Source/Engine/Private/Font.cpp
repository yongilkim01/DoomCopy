#include "pch.h"
#include "Engine/Classes/Engine/Font.h"

#ifdef _DEBUG
#pragma comment(lib, "FW1FontWrapper.lib")
#else
#pragma comment(lib, "FW1FontWrapper.lib")
#endif

IFW1Factory* FontFactory = nullptr;

class UFontFactoryCreator
{
public:
	UFontFactoryCreator()
	{
		FW1CreateFactory(FW1_VERSION, &FontFactory);
	}
	~UFontFactoryCreator()
	{
		if (nullptr != FontFactory)
		{
			FontFactory->Release();
			FontFactory = nullptr;
		}
	}
};

UFontFactoryCreator Instance;

UFont::UFont()
{
}

UFont::~UFont()
{
}

std::shared_ptr<UFont> UFont::Load(std::string_view Name, std::string_view Path)
{
	std::string UpperName = ToUpperName(Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드한 폰트입니다 " + UpperName);
		return nullptr;
	}

	std::shared_ptr<UFont> NewAsset = std::make_shared<UFont>();
	AddAsset<UFont>(NewAsset, Name, Path);
	NewAsset->LoadAsset(Path);

	return NewAsset;
}

void UFont::DrawFont(const std::string& TextValue, float FontScale, const FVector& Location, const UColor& Color, FW1_TEXT_FLAG TextFlag)
{
	std::wstring Text = UEngineString::AnsiToUnicode(TextValue);
	Font->DrawString(UGameEngine::GetDeviceContext(), Text.c_str(), FontScale, Location.X, Location.Y, Color.Color, TextFlag);
	UGameEngine::GetDeviceContext()->GSSetShader(nullptr, nullptr, 0);
}

void UFont::LoadAsset(std::string_view Path)
{
	std::wstring WPath = UEngineString::AnsiToUnicode(Path);
	D3D11_BLEND_DESC blendDesc = { 0, };
	for (int i = 0; i < 4; ++i)
	{
		blendDesc.RenderTarget[i].BlendEnable = true;
		blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
	}

	if (FontFactory->CreateFontWrapper(UGameEngine::GetDevice(), WPath.c_str(), blendDesc, &Font))
	{
		MSGASSERT("폰트 생성 실패");
		return;
	}
}
