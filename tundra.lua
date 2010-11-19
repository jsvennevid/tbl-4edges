local common = {}

Build {
	Units = "Shared/units.lua",
	SyntaxExtensions = { "tundra.syntax.glob" },
	ScriptDirs = { "Scripts" },
	Configs = {
		Config { Name = "win32-msvc", Inherit = common, Tools = { { "msvc-winsdk"; TargetArch = "x86" }, { "build.dxsdk"; TargetArch = "x86" } } },
		Config { Name = "win64-msvc", Inherit = common, Tools = { { "msvc-winsdk"; TargetArch = "x64" }, { "build.dxsdk"; TargetArch = "x64" } } },
		Config { Name = "linux-gcc", Inherit = common, Tools = { "gcc" } },
	},
}