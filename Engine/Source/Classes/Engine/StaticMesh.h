#pragma once

/**
 *	����
 */
class UStaticMesh
{
public:
	/** ������, �Ҹ��� */
	UStaticMesh();
	~UStaticMesh();

	/** ��ü �� ���� ���� */
	UStaticMesh(const UStaticMesh& Other) = delete;
	UStaticMesh(UStaticMesh&& Other) noexcept = delete;
	UStaticMesh& operator=(const UStaticMesh& Other) = delete;
	UStaticMesh& operator=(UStaticMesh&& Other) noexcept = delete;

protected:

private:

};

