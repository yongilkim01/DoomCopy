#pragma once

/**
 *	WadMap File class
 */
class UWadLine
{
public:
	/** 생성자, 소멸자 */
	UWadLine();
	~UWadLine();

	/** 객체 값 복사방지 */
	UWadLine(const UWadLine& Other) = delete;
	UWadLine(UWadLine&& Other) noexcept = delete;
	UWadLine& operator=(const UWadLine& Other) = delete;
	UWadLine& operator=(UWadLine&& Other) noexcept = delete;

	float GetStartIndex()
	{
		return StartIndex;
	}
	void SetStartIndex(float Index)
	{
		StartIndex = Index;
	}
	float GetEndIndex()
	{
		return EndIndex;
	}
	void SetEndIndex(float Index)
	{
		EndIndex = Index;
	}
	

protected:


private:
	float StartIndex = 0.0f;
	float EndIndex = 0.0f;
};

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
	void AddWadLine(UWadLine* WadLine)
	{
		WadLineVector.push_back(WadLine);
	}
	size_t GetWadLineCount()
	{
		return WadLineVector.size();
	}
	UWadLine* GetWadLineByIndex(int Index)
	{
		return WadLineVector[Index];
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
	std::vector<UWadLine*> WadLineVector;

	FVector Min;
	FVector Max;

};

