project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{Includes.Zeus}/src",
		"%{Includes.Zeus}/src/include",
		"%{Includes.glm}",
		"%{Includes.imgui}",
		"%{Includes.stb_image}",
		"%{Includes.Phyzics}",
		"%{Includes.Zerializer}"
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

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

	filter "configurations:Dist"
		optimize "On"
