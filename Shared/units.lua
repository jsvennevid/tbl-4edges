StaticLibrary {
	Name = "zenic.Base",

	Sources =
	{
		Glob { Dir = "Shared/Base/Debug/", Extensions = { ".cpp" } },
--		Glob { Dir = "Shared/Base/Io/", Extensions = ".cpp" },
--		Glob { Dir = "Shared/Base/Math/", Extensions = ".cpp" },
--		Glob { Dir = "Shared/Base/Memory/", Extensions = ".cpp" },
--		Glob { Dir = "Shared/Base/Serialize/", Extensions = ".cpp" },
--		Glob { Dir = "Shared/Base/Storage/", Extensions = ".cpp" },
--		Glob { Dir = "Shared/Base/Timer/", Extensions = ".cpp" },
	}
}

Default "zenic.Base"