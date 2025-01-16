#pragma once
#include "UMG/Public/Components/Widget.h"

/**
 *	����
 */
class UImage : public UWidget
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UImage();
	ENGINE_API ~UImage();

	/** ��ü �� ���� ���� */
	UImage(const UImage& Other) = delete;
	UImage(UImage&& Other) noexcept = delete;
	UImage& operator=(const UImage& Other) = delete;
	UImage& operator=(UImage&& Other) noexcept = delete;

	virtual void Render(UCameraComponent* CameraComponent, float DeltaTime) override;

	ENGINE_API void SetSprite(std::string_view SpriteName, UINT Index = 0);
	ENGINE_API void SetTexture(std::string_view TextureName, bool bAutoScaleValue = false, float bRatioValue = 1.0f);

	/** ��, �� �޼ҵ� */
	URenderAsset& GetRenderAsset()
	{
		return RenderAsset;
	}
	bool IsAutoScale()
	{
		return bAutoScale;
	}
	void SetAutoScale(bool Value)
	{
		bAutoScale = Value;
	}
	float GetAutoScaleRatio()
	{
		return AutoScaleRatio;
	}
	void SetAutoScaleRatio(float Value)
	{
		AutoScaleRatio = Value;
	}

protected:

private:
	URenderAsset RenderAsset;

	int CurIndex = 0;
	bool bAutoScale = false;
	float AutoScaleRatio = 1.0f;
	UPaperSprite* Sprite = nullptr;

	FWidgetColor WidgetColor;
	FWidgetUV WidgetUV;
	FWidgetData WidgetData;
};

