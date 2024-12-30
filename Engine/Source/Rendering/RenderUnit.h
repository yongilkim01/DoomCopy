#pragma once

/**
 *	설명
 */
class URenderUnit
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API URenderUnit();
	ENGINE_API ~URenderUnit();

	/** 객체 값 복사 방지 */
	URenderUnit(const URenderUnit& Other) = delete;
	URenderUnit(URenderUnit&& Other) noexcept = delete;
	URenderUnit& operator=(const URenderUnit& Other) = delete;
	URenderUnit& operator=(URenderUnit&& Other) noexcept = delete;

protected:

private:

};

