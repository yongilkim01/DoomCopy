#pragma once
#include <Engine/Classes/GameFramework/GameMode.h>

class UTileMapWindow;

/**
 *	설명
 */
class ATileMapGameMode : public AGameMode
{
public:
	/** 생성자, 소멸자 */
	ATileMapGameMode();
	~ATileMapGameMode();

	/** 객체 값 복사 방지 */
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

