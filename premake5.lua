include "Definitions.lua"

workspace "Zeus"
	architecture "x64"
	startproject "ZeusNavigator"

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

include "ZeusNavigator"
