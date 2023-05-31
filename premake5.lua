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
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/imgui/imconfig.h",
		"%{prj.name}/vendor/imgui/imgui**.h",
		"%{prj.name}/vendor/imgui/imgui**.cpp",
		"%{prj.name}/vendor/imgui/imstb**.h",
		"%{prj.name}/vendor/imgui/imgstb**.cpp",
		--"%{prj.name}/vendor/imgui/misc/**.h",
		--"%{prj.name}/vendor/imgui/misc/**.cpp",
		"%{prj.name}/vendor/imgui/backends/imgui_impl_opengl3.cpp",
		"%{prj.name}/vendor/imgui/backends/imgui_impl_opengl3.h",
		"%{prj.name}/vendor/imgui/backends/imgui_impl_glfw.cpp",
		"%{prj.name}/vendor/imgui/backends/imgui_impl_glfw.h",
		"%{prj.name}/vendor/stb_image/include/stb/**.h",
		"%{prj.name}/vendor/stb_image/include/stb/**.cpp",
		"%{prj.name}/vendor/glad/src/gl.c",
	}

	includedirs
	{
		"%{prj.name}",
		"%{prj.name}/src",
		"%{prj.name}/vendor/glad/include",
		"%{prj.name}/vendor/glm/include",
		"%{prj.name}/vendor/imgui/",
		"%{prj.name}/vendor/stb_image/include",
		"GLFW/include",
		"Zerializer/include"
	}

	links
	{
		"GLFW"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ZUES_ENGINE",
			"WINDOWING_SYSTEM_GLFW",
			"_CRT_SECURE_NO_WARNINGS"
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
		"Zeus/src/include",
		"Zeus/src",
		"Zeus/vendor/glm/include",
		"Zeus/vendor/imgui",
		"Zeus/vendor/stb_image/include",
		"Phyzics/src/include",
		"Zerializer/include"
	}

	links
	{
		"Zeus",
		"Phyzics",
		"Zerializer"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ZEUS_ENGINE",
			"WINDOWING_SYSTEM_GLFW"
		}

		postbuildcommands
		{
			("{COPY} assets %{cfg.targetdir}/assets")
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
		"%{prj.name}/src",
		"Zeus/src/include",
		"Zeus/src",
		"Zeus/vendor/glm/include",
		"Zeus/vendor/imgui",
		"Zeus/vendor/stb_image/include",
		"Phyzics/src/include",
		"Zerializer/include"
	}

	links
	{
		"Zeus",
		"Phyzics",
		"Zerializer"
	}

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
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

project "Phyzics"
	location "Phyzics"
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
		"Zeus/vendor/glm/include"
	}

	removefiles
	{
		"%{prj.name}/src/bodies/circle.cpp",
		"%{prj.name}/src/bodies/quad.cpp"
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

project "Zerializer"
	location "Zerializer"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/include"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

	filter "configurations:Dist"
		optimize "On"

project "GLFW"
	location "GLFW"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/include/GLFW/glfw3.h",
		"%{prj.name}/include/GLFW/glfw3native.h",
		"%{prj.name}/src/glfw_config.h",
		"%{prj.name}/src/context.c",
		"%{prj.name}/src/init.c",
		"%{prj.name}/src/input.c",
		"%{prj.name}/src/monitor.c",

		"%{prj.name}/src/null_init.c",
		"%{prj.name}/src/null_joystick.c",
		"%{prj.name}/src/null_monitor.c",
		"%{prj.name}/src/null_window.c",

		"%{prj.name}/src/platform.c",
		"%{prj.name}/src/vulkan.c",
		"%{prj.name}/src/window.c"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "on"

		files
		{
			"%{prj.name}/src/win32_init.c",
			"%{prj.name}/src/win32_joystick.c",
			"%{prj.name}/src/win32_module.c",
			"%{prj.name}/src/win32_monitor.c",
			"%{prj.name}/src/win32_time.c",
			"%{prj.name}/src/win32_thread.c",
			"%{prj.name}/src/win32_window.c",
			"%{prj.name}/src/wgl_context.c",
			"%{prj.name}/src/egl_context.c",
			"%{prj.name}/src/osmesa_context.c"
		}

		defines
		{
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			runtime "Release"
			symbols "on"

		filter "configurations:Dist"
			runtime "Release"
			optimize "on"

		symbols "off"
