
workspace ("InsightECS")
	architecture ("x86_64")

	configurations
	{
		"Develop-Exe",
		"Develop-Exe-Debug",
		"Develop-Exe-Release",
		"Develop-Exe-Distribution",
	}

	buildFolder = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project ("Develop")
	kind "ConsoleApp"
	location ("") -- Root Directory
	language ("C++")
	cppdialect ("C++17")
	targetname ("Develop-Target")
	

	targetdir("Binaries/" .. buildFolder)
	objdir("Binaries/Intermediates/" .. buildFolder)

	files
	{
		"Include/**.h",
		"Development/**.h",
		"Development/**.cpp",
	}

	includedirs
	{
		"Include/",
	
	}
	
filter "configurations:Develop-Exe"
	defines {"ECS_DEBUG", "ECS_DEVELOPMENT" }
	runtime "Debug"
	symbols "On"
	

filter "configurations:Develop-Exe-Debug"
	defines {"ECS_DEBUG" }
	runtime "Debug"
	symbols "On"
	
filter "configurations:Develop-Exe-Release"
	defines "ECS_RELEASE"
	runtime "Release"
	optimize "On"
	symbols "On"

filter "configurations:Develop-Exe-Distribution"
	defines "ECS_DISTRIBUTION"
	runtime "Release"
	optimize "On"
	symbols "Off"