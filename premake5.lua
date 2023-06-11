include "Definitions.lua"

workspace "Zeus"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}_%{cfg.architecture}"

group "Engine"
	include "Zeus"
	include "Zeus/vendor/GLFW"
group ""

group "Tools"
	include "Phyzics"
	include "Zerializer"
group ""

group "Test"
	include "Sandbox"
	include "SimpleGame"
group""
