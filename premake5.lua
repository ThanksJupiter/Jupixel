workspace "Jupixel"
	architecture "x86_64"
	startproject "Jupixel"

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

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Jupixel/vendor/GLFW/include"
IncludeDir["Glad"] = "Jupixel/vendor/Glad/include"
IncludeDir["ImGui"] = "Jupixel/vendor/imgui"
IncludeDir["glm"] = "Jupixel/vendor/glm"
IncludeDir["stb_image"] = "Jupixel/vendor/stb_image"

group "Dependencies"
	include "Jupixel/vendor/GLFW"
	include "Jupixel/vendor/Glad"
	include "Jupixel/vendor/imgui"

group ""

project "Jupixel"
	location "Jupixel"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"JP_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "JP_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "JP_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "JP_DIST"
		runtime "Release"
		optimize "on"
