#include "pch.h"
#include "Paths.h"
#include "WorldPartition/DebugHelpers.h"

FPaths::FPaths()
	: Path(std::filesystem::current_path())
{

}

FPaths::FPaths(std::string_view _Path)
	: Path(_Path)
{

}

FPaths::FPaths(std::filesystem::path _Path)
	: Path(_Path)
{

}

FPaths::~FPaths()
{

}

std::string FPaths::GetPathToString()
{
	return Path.string();
}

std::string FPaths::GetFileName()
{
	if (true == IsDirectory())
	{
		MSGASSERT("���� ��� �϶��� GetFileName�� ȣ���Ҽ� �ֽ��ϴ�." + Path.string());
		return "";
	}

	return Path.filename().string();
}

std::string FPaths::GetDirectoryName()
{
	if (false == IsDirectory())
	{
		MSGASSERT("���丮 ��� �϶��� GetDirectoryName�� ȣ���Ҽ� �ֽ��ϴ�." + Path.string());
		return "";
	}

	return Path.filename().string();
}

std::string FPaths::GetExtension()
{
	return Path.extension().string();
}


bool FPaths::IsExists()
{
	return std::filesystem::exists(Path);
}

bool FPaths::IsFile()
{
	return false == IsDirectory();
}

bool FPaths::IsDirectory()
{
	return std::filesystem::is_directory(Path);
}

void FPaths::MoveParent()
{
	Path = Path.parent_path();
}

void FPaths::Append(std::string_view AppendName)
{
	Path.append(AppendName);
}

bool FPaths::MoveParentToDirectory(std::string_view FindPath)
{
	FPaths DummyPath = FPaths(Path);

	if (false == DummyPath.IsDirectory())
	{
		MSGASSERT("���丮 ����϶��� MoveParentToDirectory �� ȣ���Ҽ� �ֽ��ϴ�");
		return false;
	}

	bool Result = false;
	std::filesystem::path CurPath = DummyPath.Path;

	std::filesystem::path Root = CurPath.root_path();

	while (true)
	{
		CurPath = DummyPath.Path;

		if (CurPath == Root)
		{
			break;
		}

		CurPath.append(FindPath);
		if (true == std::filesystem::exists(CurPath))
		{
			Result = true;
			Path = CurPath;
			break;
		}
		DummyPath.MoveParent();
	}

	return Result;
}

bool FPaths::Move(std::string_view Path)
{
	Append(Path);

	if (false == IsExists())
	{
		MSGASSERT("�������� �ʴ� ��η� �̵��Ϸ��� �߽��ϴ�.");
		return false;
	}

	return true;
}
