#pragma once

/**
 *	설명
 */
class USkeletalMesh
{
public:
	/** 생성자, 소멸자 */
	USkeletalMesh();
	~USkeletalMesh();

	/** 객체 값 복사 방지 */
	USkeletalMesh(const USkeletalMesh& Other) = delete;
	USkeletalMesh(USkeletalMesh&& Other) noexcept = delete;
	USkeletalMesh& operator=(const USkeletalMesh& Other) = delete;
	USkeletalMesh& operator=(USkeletalMesh&& Other) noexcept = delete;

protected:

private:

};

