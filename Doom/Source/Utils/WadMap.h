#pragma once

/**
 *	WadMap File class
 */
class UWadMap
{
public:
	/** ������, �Ҹ��� */
	UWadMap();
	~UWadMap();

	/** ��ü �� ������� */
	UWadMap(const UWadMap& Other) = delete;
	UWadMap(UWadMap&& Other) noexcept = delete;
	UWadMap& operator=(const UWadMap& Other) = delete;
	UWadMap& operator=(UWadMap&& Other) noexcept = delete;

	void AddVertex(FVector Vertex)
	{
		VertexVector.push_back(Vertex);
	}
	size_t GetVertexCount()
	{
		return VertexVector.size();
	}

protected:


private:
	std::vector<FVector> VertexVector;

};

