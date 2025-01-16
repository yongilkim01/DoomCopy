#pragma once
#include "UMG/Public/Components/Widget.h"
#include "Engine/Classes/Engine/Font.h"

/**
 *	����
 */
class UTextBox : public UWidget
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UTextBox();
	ENGINE_API ~UTextBox();

	/** ��ü �� ���� ���� */
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

