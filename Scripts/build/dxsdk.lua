module(..., package.seeall)

local native = require "tundra.native"
local os = require "os"

if native.host_platform ~= "windows" then
	error("dxsdk can only be used on windows hosts")
end

function apply(env, options)
	options = options or {}
	local target_arch = options.TargetArch or "x86"
	local sdk_path = options.CustomPath or env:get_external_env_var("DXSDK_DIR")

	local orig_include_path = env:get_external_env_var("INCLUDE")
	env:set_external_env_var("INCLUDE", sdk_path .. "\\Include;" .. orig_include_path)

	local lib_dir = "Lib\\x86"

	if "x64" == target_arch then
		lib_dir = "Lib\\x64"
	end

	local orig_lib_path = env:get_external_env_var("LIBPATH")
	local out_lib_path = sdk_path .. "\\" .. lib_dir .. ";" .. orig_lib_path
	env:set_external_env_var("LIB", out_lib_path)
	env:set_external_env_var("LIBPATH", out_lib_path)
end
