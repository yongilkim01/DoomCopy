#pragma once
#include <GameFramework/Actor.h>

class ULineComponent;

/**
 *	����
 */
class ADrawLineActor : public AActor
{
public:
	/** ������, �Ҹ��� */
	ADrawLineActor();
	~ADrawLineActor();

	/** ��ü �� ���� ���� */
	ADrawLineActor(const ADrawLineActor& Other) = delete;
	ADrawLineActor(ADrawLineActor&& Other) noexcept = delete;
	ADrawLineActor& operator=(const ADrawLineActor& Other) = delete;
	ADrawLineActor& operator=(ADrawLineActor&& Other) noexcept = delete;

protected:
	/** ���� ��� �޼ҵ� */
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	std::shared_ptr<ULineComponent> LineComponent;

};

