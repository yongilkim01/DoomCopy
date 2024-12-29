#pragma once

/**
 *	WadMap File class
 */
class UWadMap
{
public:
	/** 생성자, 소멸자 */
	UWadMap();
	~UWadMap();

	/** 객체 값 복사방지 */
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
	FVector GetVertexByIndex(int Index)
	{
		return VertexVector[Index];
	}

	FVector GetMinVector()
	{
		return Min;
	}
	FVector GetMaxVector()
	{
		return Max;
	}
	void SetMinX(float X)
	{
		Min.X = X;
	}
	void SetMinY(float Y)
	{
		Min.Y = Y;
	}
	void SetMaxX(float X)
	{
		Max.X = X;
	}
	void SetMaxY(float Y)
	{
		Max.Y = Y;
	}

protected:


private:
	std::vector<FVector> VertexVector;

	FVector Min;
	FVector Max;

};

