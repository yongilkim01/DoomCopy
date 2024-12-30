#pragma once
#include <GameFramework/Actor.h>

class UPrimitiveComponent;
class UPaperSpriteComponent;
class ULineComponent;
class UWad;
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
	std::vector<std::shared_ptr<UPaperSpriteComponent>> SquareComponentVector;
	std::vector<std::shared_ptr<ULineComponent>> LineComponentVector;
	UWad* Wad = nullptr;

};

