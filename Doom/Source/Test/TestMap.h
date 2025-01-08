#pragma once
#include <GameFramework/Actor.h>

class UTestComponent;
class UPaperSpriteComponent;

/**
 *	����
 */
class ATestMap : public AActor
{
public:
	/** ������, �Ҹ��� */
	ATestMap();
	~ATestMap();

	/** ��ü �� ���� ���� */
	ATestMap(const ATestMap& Other) = delete;
	ATestMap(ATestMap&& Other) noexcept = delete;
	ATestMap& operator=(const ATestMap& Other) = delete;
	ATestMap& operator=(ATestMap&& Other) noexcept = delete;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;
};

