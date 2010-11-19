local filters = {
	{ Pattern = "[Pp]sp"; Config = "psp-*-*-*" },
	{ Pattern = "[Pp]s2"; Config = "ps2-*-*-*" },
	{ Pattern = "[Ww]in32"; Config = "win32-*-*-*" },
	{ Pattern = "[Tt]est"; Config = "*-*-*-test" },
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
	Sources = {
		FGlob {
			Dir = "Shared/Geometry/",
			Extensions = { ".cpp" },
			Filters = {
				{ Pattern = "[Oo]pen[Gg][Ll]"; Config = "win32-*-*-*" },
				{ Pattern = "[Dd][Xx]9"; Config = "win32-*-*-*" },
				{ Pattern = ".vsm"; Config = "ps2-*-*-*" },
				{ Pattern = "[Tt]est"; Config = "*-*-*-test" }
			}
		}
	},
	Depends = { "zenic.Base", "External.ps2sdk" }
}

StaticLibrary {
	Name = "zenic.Graphics.Image",
	Sources = { ZGlob("Shared/Graphics/Image/") },
	Depends = { "zenic.Base", "External.ps2sdk" }
}

StaticLibrary {
	Name = "zenic.Graphics.Renderer",
	Sources = {
		FGlob {
			Dir = "Shared/Graphics/Renderer/",
			Extensions = { ".cpp", ".vsm" },
			Filters = {
				{ Pattern = "[Oo]pen[Gg][Ll]"; Config = "win32-*-*-*" },
				{ Pattern = "[Dd][Xx]9"; Config = "win32-*-*-*" },
				{ Pattern = ".vsm"; Config = "ps2-*-*-*" },
				{ Pattern = "[Tt]est"; Config = "*-*-*-test" }
			}
		}
	},
	Depends = { "zenic.Base", "External.ps2sdk", "External.ps2emu" }
}

StaticLibrary {
	Name = "zenic.Graphics.Scene",
	Sources = { ZGlob("Shared/Graphics/Scene/") },
	Depends = { "zenic.Base", "External.ps2sdk" }
}

StaticLibrary {
	Name = "zenic.Hardware",
	Sources = {
		FGlob {
			Dir = "Shared/Hardware/",
			Extensions = { ".cpp" },
			Filters = {
				{ Pattern = "[Oo]pen[Gg][Ll]"; Config = "win32-*-*-*" },
				{ Pattern = "[Dd][Xx]9"; Config = "win32-*-*-*" },
				{ Pattern = ".vsm"; Config = "ps2-*-*-*" },
				{ Pattern = "[Tt]est"; Config = "*-*-*-test" }
			}
		}
	},
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
		}
	}
}

StaticLibrary {
	Name = "External.ps2emu",
	Config = "win32-*-*-*",
	
	Sources = {
		Glob { Dir = "Shared/External/ps2emu", Extensions = { ".cpp" } }
	}
}

Program {
	Name = "4edges",

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
		{ "c", "gcc", "debug", "kernel", "syscall", "ps2snd", "pad", "m", "fileXio"; Config = "ps2-*-*-*" },
		{ "d3d9.lib", "d3dx9.lib", "opengl32.lib", "glu32.lib", "gdi32.lib", "user32.lib"; Config = "win32-*-*-*" }
	}
}

Default "4edges"
