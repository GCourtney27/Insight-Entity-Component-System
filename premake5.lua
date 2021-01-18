
workspace ("InsightECS")
	architecture ("x86_64")

	configurations
	{
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

filter "configurations:Develop-Exe-Debug"
	defines "_DEBUG"
	runtime "Debug"
	symbols "On"
	
filter "configurations:Develop-Exe-Release"
	defines "_RELEASE"
	runtime "Release"
	optimize "On"
	symbols "On"

filter "configurations:Develop-Exe-Distribution"
	defines "_DISTRIBUTION"
	runtime "Release"
	optimize "On"
	symbols "Off"