#pragma once
#include <GameFramework/Actor.h>

class UDoomMapComponent;

/**
 *	���� 
 */
class ADrawSquareActor : public AActor
{
public:
	/** ������, �Ҹ��� */
	ADrawSquareActor();
	~ADrawSquareActor();

	/** ��ü �� ���� ���� */
	ADrawSquareActor(const ADrawSquareActor& Other) = delete;
	ADrawSquareActor(ADrawSquareActor&& Other) noexcept = delete;
	ADrawSquareActor& operator=(const ADrawSquareActor& Other) = delete;
	ADrawSquareActor& operator=(ADrawSquareActor&& Other) noexcept = delete;

protected:
	/** ���� ��� �޼ҵ� */
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	std::shared_ptr<UDoomMapComponent> SquareComponent;

};

