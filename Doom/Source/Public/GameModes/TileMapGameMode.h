#pragma once
#include <Engine/Classes/GameFramework/GameMode.h>

class UTileMapWindow;

/**
 *	����
 */
class ATileMapGameMode : public AGameMode
{
public:
	/** ������, �Ҹ��� */
	ATileMapGameMode();
	~ATileMapGameMode();

	/** ��ü �� ���� ���� */
	ATileMapGameMode(const ATileMapGameMode& Other) = delete;
	ATileMapGameMode(ATileMapGameMode&& Other) noexcept = delete;
	ATileMapGameMode& operator=(const ATileMapGameMode& Other) = delete;
	ATileMapGameMode& operator=(ATileMapGameMode&& Other) noexcept = delete;

protected:
	virtual void Tick(float DeltaTime) override;;
	virtual void LevelChangeStart() override;

private:
	std::shared_ptr<UTileMapWindow> TileMapWindow;
};

