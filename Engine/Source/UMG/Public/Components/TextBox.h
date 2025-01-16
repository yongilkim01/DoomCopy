#pragma once
#include "UMG/Public/Components/Widget.h"
#include "Engine/Classes/Engine/Font.h"

/**
 *	설명
 */
class UTextBox : public UWidget
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UTextBox();
	ENGINE_API ~UTextBox();

	/** 객체 값 복사 방지 */
	UTextBox(const UTextBox& Other) = delete;
	UTextBox(UTextBox&& Other) noexcept = delete;
	UTextBox& operator=(const UTextBox& Other) = delete;
	UTextBox& operator=(UTextBox&& Other) noexcept = delete;

	virtual void Render(UCameraComponent* CameraComponent, float DeltaTime) override;

	ENGINE_API void SetFont(
		std::string_view Value, 
		float Scale = 20.0f, 
		UColor Color = UColor::BLACK,
		FW1_TEXT_FLAG TextFlag = FW1_TEXT_FLAG::FW1_LEFT);

	void SetText(std::string Value)
	{
		Text = Value;
	}

protected:

private:
	UFont* Font = nullptr;
	std::string Text;
	float Scale = 20.0f;
	UColor Color = UColor::BLACK;
	FW1_TEXT_FLAG Flag = FW1_TEXT_FLAG::FW1_LEFT;

};

