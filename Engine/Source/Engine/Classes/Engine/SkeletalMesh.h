#pragma once

/**
 *	����
 */
class USkeletalMesh
{
public:
	/** ������, �Ҹ��� */
	USkeletalMesh();
	~USkeletalMesh();

	/** ��ü �� ���� ���� */
	USkeletalMesh(const USkeletalMesh& Other) = delete;
	USkeletalMesh(USkeletalMesh&& Other) noexcept = delete;
	USkeletalMesh& operator=(const USkeletalMesh& Other) = delete;
	USkeletalMesh& operator=(USkeletalMesh&& Other) noexcept = delete;

protected:

private:

};

