#pragma once
#include "Engine/Classes/GameFramework/Actor.h"

class UWidget;
class UCameraComponent;

/**
 *	����
 */
class AHUD : public AActor
{
	friend class ULevel;
public:
	/** ������, �Ҹ��� */
	ENGINE_API AHUD();
	ENGINE_API ~AHUD();

	/** ��ü �� ���� ���� */
	AHUD(const AHUD& Other) = delete;
	AHUD(AHUD&& Other) noexcept = delete;
	AHUD& operator=(const AHUD& Other) = delete;
	AHUD& operator=(AHUD&& Other) noexcept = delete;

	template<typename WidgetType>
	std::shared_ptr<WidgetType> CreateWidget(int ZOrder, std::string_view WidgetName = "NONE")
	{
		static_assert(std::is_base_of_v<UWidget, WidgetType>, "������ ��ӹ��� ���� Ŭ������ CreateWidget�Ϸ��� �߽��ϴ�");

		if (false == std::is_base_of_v<UWidget, WidgetType>)
		{
			MSGASSERT("������ ��ӹ��� ���� Ŭ������ CreateWidget�Ϸ��� �߽��ϴ�");
			return nullptr;
		}

		char* Memory = new char[sizeof(WidgetType)];

		UWidget* WidgetPtr = reinterpret_cast<WidgetType*>(Memory);

		WidgetPtr->HUD = this;

		WidgetType* NewPtr = reinterpret_cast<WidgetType*>(Memory);

		std::shared_ptr<WidgetType> NewWidgetPtr(NewPtr = new(Memory) WidgetType());

		NewWidgetPtr->SetName(WidgetName);
		NewWidgetPtr->SetOrder(ZOrder);

		WidgetMap[NewWidgetPtr->GetOrder()].push_back(NewWidgetPtr);

		return NewWidgetPtr;
	}

protected:

private:
	std::map<int, std::list<std::shared_ptr<UWidget>>> WidgetMap;

	void RenderUI(UCameraComponent* CameraComponent, float DeltaTime);
};

