workspace "Zeus"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Zeus"
	location "Zeus"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}",
		"%{prj.name}/src",
		"%{prj.name}/vendor/glad/include",
		"%{prj.name}/vendor/glm/include",
		"%{prj.name}/vendor/imgui/",
		"%{prj.name}/vendor/stb_image/include",
		"%{prj.name}/dependency/glfw/include",
		"%{prj.name}/dependency/zerializer/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ZUES_ENGINE",
			"WINDOWING_SYSTEM_GLFW"
		}

		--postbuildcommands
		--{
		--	("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		--}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

	filter "configurations:Dist"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Zeus"
	}

	links
	{
		"Zeus"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ZUES_ENGINE",
			"WINDOWING_SYSTEM_GLFW"
		}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

	filter "configurations:Dist"
		optimize "On"

project "SimpleGame"
	location "SimpleGame"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Zeus/src/include",
		"Zeus/src",
		"Zeus/vendor/glm/include",
		"Zeus/vendor/imgui",
		"Zeus/vendor/stb_image/include"
	}

	links
	{
		"Zeus"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ZUES_ENGINE",
			"WINDOWING_SYSTEM_GLFW"
		}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

	filter "configurations:Dist"
		optimize "On"