#pragma once
#include "Engine/Classes/Engine/EngineResource.h"

#include "ThirdParty/FW1FontWrapper/Inc/FW1FontWrapper.h"

/**
 *	설명
 */
class UFont : public UResource
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UFont();
	ENGINE_API ~UFont();

	/** 객체 값 복사 방지 */
	UFont(const UFont& Other) = delete;
	UFont(UFont&& Other) noexcept = delete;
	UFont& operator=(const UFont& Other) = delete;
	UFont& operator=(UFont&& Other) noexcept = delete;

	ENGINE_API static std::shared_ptr<UFont> Load(std::string_view Name, std::string_view Path);

	void DrawFont(
		const std::string& TextValue,
		float FontScale,
		const FVector& Location,
		const UColor& Color = UColor::BLACK,
		FW1_TEXT_FLAG TextFlag = FW1_LEFT);

protected:

private:
	void LoadAsset(std::string_view Path);

	Microsoft::WRL::ComPtr<IFW1FontWrapper> Font;

};

