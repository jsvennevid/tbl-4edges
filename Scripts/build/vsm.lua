--
-- This software is supplied under the terms of a license agreement or
-- agreement and may not be copied or disclosed except in
-- with the terms of that agreement.
--
-- Copyright (c) 2010 Jesper Svennevid, Daniel Collin.
-- Rights Reserved.
--

module(..., package.seeall)

-- Really need to cleanup this

local nodegen = require "tundra.nodegen"
local boot = require "tundra.boot"
local util = require "tundra.util"
local path = require "tundra.path"
local native = require("tundra.native")

local function get_cpp_scanner(env, fn)
	local function new_scanner()
		local paths = util.map(env:get_list("CPPPATH"), function (v) return env:interpolate(v) end)
		return boot.GlobalEngine:make_cpp_scanner(paths)
	end
	return env:memoize("CPPPATH", "_cpp_scanner", new_scanner)
end

local function generic_cpp_setup(env)
	local _anyc_compile = function(env, pass, fn, label, action)
		local object_fn
		local pch_input = env:get('_PCH_FILE', '')

		-- Drop leading $(OBJECTDIR)[/\\] in the input filename.
		do
			local pname = fn:match("^%$%(OBJECTDIR%)[/\\](.*)$")
			if pname then
				object_fn = pname
			else
				object_fn = fn
			end
		end

		-- Compute path under OBJECTDIR we want for the resulting object file.
		-- Replace ".." with "dotdot" to avoid creating files outside the
		-- object directory.
		do
			local relative_name = path.drop_suffix(object_fn:gsub("%.%.", "dotdot"))
			object_fn = "$(OBJECTDIR)/$(UNIT_PREFIX)/" .. relative_name .. '$(OBJECTSUFFIX)'
		end

		local implicit_inputs = nil

		return env:make_node {
			Label = label .. ' $(@)',
			Pass = pass,
			Action = action,
			InputFiles = { fn },
			OutputFiles = { object_fn },
			ImplicitInputs = implicit_inputs,
			Scanner = get_cpp_scanner(env, fn),
		}
	end

	local mappings = {
		["VSMEXTS"] = { Label="VsmAsm", Action="$(VSMCOM)" },
	}

	for key, setup in pairs(mappings) do
		for _, ext in ipairs(env:get_list(key)) do
			env:register_implicit_make_fn(ext, function(env, pass, fn)
				return _anyc_compile(env, pass, fn, setup.Label, setup.Action)
			end)
		end
	end
end

function apply(env, options)
	tundra.boot.load_toolset("generic-cpp", env, options)

	-- todo cleanup

	env:add_setup_function(generic_cpp_setup)

	env:set_many {
		["VSMEXTS"] = { ".vsm" },
		["OBJECTSUFFIX"] = ".o",
		["VSMAS"] = "dvp-as",
		["VSMCOM"] = "$(VSMAS) -o $(@) $(<)",
	}
end

