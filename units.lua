local filters = {
	{ Pattern = "[Pp]sp"; Config = "psp-*-*-*" },
	{ Pattern = "[Pp]s2"; Config = "ps2-*-*-*" },
	{ Pattern = "[Ww]in32"; Config = "win32-*-*-*" },
	{ Pattern = "[Tt]est"; Config = "*-*-*-test" }
}

local function ZGlob(path)
	return FGlob { Dir = path, Extensions = { ".cpp" }, Filters = filters }
end

StaticLibrary {
	Name = "zenic.Audio",
	Sources = { ZGlob("Shared/Audio/") },
	Depends = { "zenic.Base" }
}

StaticLibrary {
	Name = "zenic.Base",

	Sources = { ZGlob("Shared/Base/") },

	Env = { CPPPATH = "." },

	Propagate = {
		Env = { CPPPATH = "." }
	}
}

StaticLibrary {
	Name = "zenic.Geometry",
	Sources = { ZGlob("Shared/Geometry/") },
	Depends = { "zenic.Base" }
}

StaticLibrary {
	Name = "zenic.Graphics.Image",
	Sources = { ZGlob("Shared/Graphics/Image/") },
	Depends = { "zenic.Base" }
}

StaticLibrary {
	Name = "zenic.Graphics.Renderer",
	Sources = { ZGlob("Shared/Graphics/Renderer/") },
	Depends = { "zenic.Base" }
}

StaticLibrary {
	Name = "zenic.Graphics.Scene",
	Sources = { ZGlob("Shared/Graphics/Scene/") },
	Depends = { "zenic.Base" }
}

StaticLibrary {
	Name = "zenic.Hardware",
	Sources = { ZGlob("Shared/Hardware/") },
	Depends = { "zenic.Base" }
}

StaticLibrary {
	Name = "zenic.Input",
	Sources = { ZGlob("Shared/Input/") },
	Depends = { "zenic.Base" }
}

StaticLibrary {
	Name = "zenic.Misc",
	Sources = { ZGlob("Shared/Misc/") },
	Depends = { "zenic.Base" }
}

StaticLibrary {
	Name = "zenic.Net",
	Sources = { ZGlob("Shared/Net/") },
	Depends = { "zenic.Base" }
}

StaticLibrary {
	Name = "zenic.Physics",
	Sources = { ZGlob("Shared/Physics/") },
	Depends = { "zenic.Base" }
}

Program {
	Name = "cubic",

	Sources = { ZGlob("Projects/Bp06Demo/") },

	Depends = {
		"zenic.Audio",
		"zenic.Base",
		"zenic.Geometry",
		"zenic.Graphics.Image", "zenic.Graphics.Renderer", "zenic.Graphics.Scene",
		"zenic.Hardware",
		"zenic.Input",
		"zenic.Misc",
		"zenic.Net",
		"zenic.Physics",
	}
}

Default "cubic"
