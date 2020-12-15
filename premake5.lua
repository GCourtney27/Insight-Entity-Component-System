
workspace ("InsightECS")
	architecture ("x86_64")

	configurations
	{
		"Develop-Exe",
	}

	buildFolder = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project ("Develop")
	filter "configurations:*Exe"
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
	defines "_DEVELOPMENT"
	runtime "Debug"
	symbols "on"
	
filter "configurations:Develop-Lib"
	defines "_RELEASE"
	runtime "Release"
	optimize "on"
	symbols "on"