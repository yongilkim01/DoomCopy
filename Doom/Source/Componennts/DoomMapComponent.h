#pragma once
#include <Classes/Components/PrimitiveComponent.h>

/**
 *	����
 */
class UDoomMapComponent : public UPrimitiveComponent
{
public:
	/** ������, �Ҹ��� */
	UDoomMapComponent();
	~UDoomMapComponent();

	/** ��ü �� ���� ���� */
	UDoomMapComponent(const UDoomMapComponent& Other) = delete;
	UDoomMapComponent(UDoomMapComponent&& Other) noexcept = delete;
	UDoomMapComponent& operator=(const UDoomMapComponent& Other) = delete;
	UDoomMapComponent& operator=(UDoomMapComponent&& Other) noexcept = delete;

protected:


private:

};

