#pragma once
#include "GameFramework/Actor.h"

class ATitleLogo;

/**
 *	타이틀 게임모드 클래스
 */
class ATitleGameMode : public AActor
{
public:
	/** 생성자, 소멸자 */
	ATitleGameMode();
	~ATitleGameMode();

	/** 객체 값 복사 방지 */
	ATitleGameMode(const ATitleGameMode& Other) = delete;
	ATitleGameMode(ATitleGameMode&& Other) noexcept = delete;
	ATitleGameMode& operator=(const ATitleGameMode& Other) = delete;
	ATitleGameMode& operator=(ATitleGameMode&& Other) noexcept = delete;

	void Tick(float DeltaTime);

protected:

private:
	std::shared_ptr<ATitleLogo> TitleLogo;

};