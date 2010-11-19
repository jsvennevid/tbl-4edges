local common = {
	Env = {
		CPPDEFS = {
			{ "ZENIC_WIN32", "PS2_EMU"; Config = "win32-*-*-*" },
			{ "ZENIC_PS2"; Config = "ps2-*-*-*" },

			{ "ZENIC_DEBUG"; Config = "*-*-debug-*" },
			{ "ZENIC_RELEASE"; Config = "*-*-release-*" },
			{ "ZENIC_FINAL"; Config = "*-*-production-*" },
		}
	}
}

Build {
	Units = "units.lua",
	SyntaxExtensions = { "tundra.syntax.glob" },
	ScriptDirs = { "Scripts" },
	Configs = {
		Config { Name = "win32-msvc", Inherit = common, Tools = { { "msvc-winsdk"; TargetArch = "x86" }, { "build.dxsdk"; TargetArch = "x86" } } },
		Config { Name = "win64-msvc", Inherit = common, Tools = { { "msvc-winsdk"; TargetArch = "x64" }, { "build.dxsdk"; TargetArch = "x64" } } },
		Config { Name = "linux-gcc", Inherit = common, Tools = { "gcc" } },
	},
}