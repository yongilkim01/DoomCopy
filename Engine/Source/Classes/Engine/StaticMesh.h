#pragma once

/**
 *	설명
 */
class UStaticMesh
{
public:
	/** 생성자, 소멸자 */
	UStaticMesh();
	~UStaticMesh();

	/** 객체 값 복사 방지 */
	UStaticMesh(const UStaticMesh& Other) = delete;
	UStaticMesh(UStaticMesh&& Other) noexcept = delete;
	UStaticMesh& operator=(const UStaticMesh& Other) = delete;
	UStaticMesh& operator=(UStaticMesh&& Other) noexcept = delete;

protected:

private:

};

