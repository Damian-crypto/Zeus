project "Zeus"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-intermediate/" .. outputdir .. "/%{prj.name}")
	
	pchheader "corepch.h"
	pchsource "corepch.cpp"

	files
	{
		"%{wks.location}/Zeus/src/**.h",
		"%{wks.location}/Zeus/src/**.cpp",
		"%{wks.location}/Zeus/vendor/imgui/imconfig.h",
		"%{wks.location}/Zeus/vendor/imgui/imgui**.h",
		"%{wks.location}/Zeus/vendor/imgui/imgui**.cpp",
		"%{wks.location}/Zeus/vendor/imgui/imstb**.h",
		"%{wks.location}/Zeus/vendor/imgui/imgstb**.cpp",
		"%{wks.location}/Zeus/vendor/imgui/backends/imgui_impl_opengl3.cpp",
		"%{wks.location}/Zeus/vendor/imgui/backends/imgui_impl_opengl3.h",
		"%{wks.location}/Zeus/vendor/imgui/backends/imgui_impl_glfw.cpp",
		"%{wks.location}/Zeus/vendor/imgui/backends/imgui_impl_glfw.h",
		"%{wks.location}/Zeus/vendor/stb_image/include/stb/**.h",
		"%{wks.location}/Zeus/vendor/stb_image/include/stb/**.cpp",
		"%{wks.location}/Zeus/vendor/glad/src/gl.c",
		"%{wks.location}/Zeus/corepch.h",
		"%{wks.location}/Zeus/corepch.cpp"
	}

	includedirs
	{
		"%{wks.location}/Zeus",
		"%{wks.location}/Zeus/src",
		"%{wks.location}/Zeus/vendor/glad/include",
		"%{wks.location}/Zeus/vendor/glm/include",
		"%{wks.location}/Zeus/vendor/imgui/",
		"%{wks.location}/Zeus/vendor/stb_image/include",
		"%{wks.location}/Zeus/vendor/GLFW/include"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "files:**.c"
		flags { "NoPCH" }

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

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		runtime "Release"
		optimize "On"
