project "SPIRV-Reflect"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime "off"
	warnings "Off"
	
	VULKAN_SDK = os.getenv("VULKAN_SDK")
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	disablewarnings 
	{
		"6385",
		"6011",
		"6386",
		"6487"
	}
	files
	{
		"spirv_reflect.h",
		"spirv_reflect.cpp",
		"spirv_reflect.c",
	}
	includedirs
    {
		"%{VULKAN_SDK}/Include/"
    }
	linkoptions 
	{
		"/ignore:4006",
		"/ignore:4099"
	}
	defines
	{
		"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
        optimize "off"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
        symbols "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"
        symbols "off"
