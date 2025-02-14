#include "Engine.h"
#include "Core/Public/Misc/Paths.h"
#include "Core/Public/Debugging/DebugMacros.h"

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
		MSGASSERT("파일 경로 일때만 GetFileName을 호출할수 있습니다." + Path.string());
		return "";
	}

	return Path.filename().string();
}

std::string FPaths::GetDirectoryName()
{
	if (false == IsDirectory())
	{
		MSGASSERT("디렉토리 경로 일때만 GetDirectoryName을 호출할수 있습니다." + Path.string());
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
		MSGASSERT("디렉토리 경로일때만 MoveParentToDirectory 를 호출할수 있습니다");
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

bool FPaths::MoveEngineShaderDirectory()
{
	if (true == MoveEngineDirectory())
	{
		FPaths DummyPath = FPaths(Path);

		if (false == DummyPath.IsDirectory())
		{
			MSGASSERT("디렉토리 경로가 이닙니다");
			return false;
		}
		std::filesystem::path CurPath = DummyPath.Path;

		CurPath = DummyPath.Path;

		CurPath.append("Shaders");

		if (true == std::filesystem::exists(CurPath))
		{
			Path = CurPath;
			return true;
		}
	}
	
	return false;
}

bool FPaths::MoveEngineDirectory()
{
	FPaths DummyPath = FPaths(Path);

	if (false == DummyPath.IsDirectory())
	{
		MSGASSERT("디렉토리 경로가 이닙니다");
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

		CurPath.append("Engine");

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

bool FPaths::MoveSelectDirectory(std::string_view DirectoryName)
{
	FPaths DummyPath = FPaths(Path);

	if (false == DummyPath.IsDirectory())
	{
		MSGASSERT("디렉토리 경로가 이닙니다");
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

		CurPath.append(DirectoryName);

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

bool FPaths::MoveParentToDirectory(std::string_view FindDirectoryName, std::string_view ParentDirectoryName)
{
	FPaths DummyPath = FPaths(Path);

	if (false == DummyPath.IsDirectory())
	{
		MSGASSERT("디렉토리 경로가 이닙니다");
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

		std::string FindPath = ParentDirectoryName.data();
		FindPath.append("\\");
		FindPath.append(FindDirectoryName);

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

bool FPaths::MoveSelectShaderDirectory(std::string_view DirectoryName)
{
	if (true == MoveSelectDirectory(DirectoryName))
	{
		FPaths DummyPath = FPaths(Path);

		if (false == DummyPath.IsDirectory())
		{
			MSGASSERT("디렉토리 경로가 이닙니다");
			return false;
		}
		std::filesystem::path CurPath = DummyPath.Path;

		CurPath = DummyPath.Path;

		CurPath.append("Shaders");

		if (true == std::filesystem::exists(CurPath))
		{
			Path = CurPath;
			return true;
		}
	}

	return false;
}

bool FPaths::Move(std::string_view Path)
{
	Append(Path);

	if (false == IsExists())
	{
		MSGASSERT("존재하지 않는 경로로 이동하려고 했습니다.");
		return false;
	}

	return true;
}
