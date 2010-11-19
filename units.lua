local filters = {
	{ Pattern = "[Pp]sp"; Config = "psp-*-*-*" },
	{ Pattern = "[Pp]s2"; Config = "ps2-*-*-*" },
	{ Pattern = "[Ww]in32"; Config = "win32-*-*-*" },
	{ Pattern = "[Tt]est"; Config = "*-*-*-test" },
	{ Pattern = "[Oo]pen[Gg][Ll]"; Config = "win32-*-*-*" },
	{ Pattern = "[Dd][Xx]9"; Config = "win32-*-*-*" }
}

local function ZGlob(path, exts)
	if type(exts) == "nil" then
		exts = { ".cpp" }
	end
	return FGlob { Dir = path, Extensions = exts, Filters = filters }
end

StaticLibrary {
	Name = "zenic.Audio",
	Sources = { ZGlob("Shared/Audio/") },
	Depends = { "zenic.Base", "External.ps2sdk" }
}

StaticLibrary {
	Name = "zenic.Base",

	Sources = { ZGlob("Shared/Base/") },

	Env = { CPPPATH = "." },

	Propagate = {
		Env = { CPPPATH = "." },
	},

	Depends =
	{
		"External.ps2sdk"
	}
}

StaticLibrary {
	Name = "zenic.Geometry",
	Sources = { ZGlob("Shared/Geometry/") },
	Depends = { "zenic.Base", "External.ps2sdk" }
}

StaticLibrary {
	Name = "zenic.Graphics.Image",
	Sources = { ZGlob("Shared/Graphics/Image/") },
	Depends = { "zenic.Base", "External.ps2sdk" }
}

StaticLibrary {
	Name = "zenic.Graphics.Renderer",
	Sources = { ZGlob("Shared/Graphics/Renderer/", { ".cpp", ".vsm" } ) },
	Depends = { "zenic.Base", "External.ps2sdk" }
}

StaticLibrary {
	Name = "zenic.Graphics.Scene",
	Sources = { ZGlob("Shared/Graphics/Scene/") },
	Depends = { "zenic.Base", "External.ps2sdk" }
}

StaticLibrary {
	Name = "zenic.Hardware",
	Sources = { ZGlob("Shared/Hardware/") },
	Depends = { "zenic.Base", "External.ps2sdk" }
}

StaticLibrary {
	Name = "zenic.Input",
	Sources = { ZGlob("Shared/Input/") },
	Depends = { "zenic.Base", "External.ps2sdk" }
}

StaticLibrary {
	Name = "zenic.Misc",
	Sources = { ZGlob("Shared/Misc/") },
	Depends = { "zenic.Base", "External.ps2sdk" }
}

ExternalLibrary {
	Name = "External.ps2sdk",
	Config = "ps2-*-*-*",

	Propagate = {
		Env = {
			CPPPATH = {
				"Shared/External/ps2sdk/ee/include",
				"Shared/External/ps2sdk/common/include"
			},

			LIBPATH = {
				"Shared/External/ps2sdk/ee/lib"
			},


			PROGOPTS = {
				"-T", "Shared/External/ps2sdk/ee/startup/linkfile"
			}
		},

		Libs = {
			"c", "gcc", "debug", "kernel", "syscall", "ps2snd"
		}
	}
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
		"External.ps2sdk"
	},

	Libs = {
		{ "stdc++"; Config = "*-gcc-*-*" },
		{ "c", "gcc", "debug", "kernel", "syscall", "ps2snd", "pad", "m", "fileXio"; Config = "ps2-*-*-*" }
	}
}

Default "cubic"
