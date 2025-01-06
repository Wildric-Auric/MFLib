
workspace "MFLib"
	configurations {"Release", "Debug"}
	architecture "x64"
	language "C++"
    cppdialect "C++11"
	targetdir "build/bin/%{prj.name}/%{cfg.buildcfg}"
    objdir    "build/bin/objs"
    characterset("MBCS")
    buildoptions { "/EHsc" }
	location "build"
	project "MFLib"
	kind "ConsoleApp"
	includedirs {
		"src/",
	}
	files {
		"src/**.cpp",
		"src/**.h",
		"src/**.hpp",
		"premake5.lua",
	}
	links {

	}
	filter "configurations:Release"
		optimize "On"
	filter "configurations:Debug"
		symbols "On"
