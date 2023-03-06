project "Hydra"
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    disablewarnings { "4996", "6285", "26437", "26451", "26498", "26800", "26495", "26439" }
    warnings "Default"
    debugdir "../AssetData/"
    
    pchheader "HYpch.h"
	pchsource "src/HYpch.cpp"
    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")
    files
    {
        "src/**.cpp",
        "src/**.h", 
        "vendor/dx12Helpers/**.cpp",
        FileDir["VMA"]
    }
    
    includedirs
    {
        "src",
        IncludeDir["GLFW"],
        IncludeDir["spdlog"],
        IncludeDir["VulkanSDK"],
        IncludeDir["Dx12Helpers"],
        IncludeDir["Vendor"]
    }
    
    libdirs
    {
    }

    linkoptions 
	{
		"/ignore:4006",
		"/ignore:4099"
	}
    
    defines {"_CONSOLE"}
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        defines
        {
            "HY_CONFIG_DEBUG"
        }
       
    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        defines
        {
            "HY_CONFIG_RELEASE"
        }

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"
        symbols "off"
        defines
        {
            "HY_CONFIG_DIST"
        }

    filter "system:windows"
        symbols "On"		
        systemversion "latest"
    
        flags 
        { 
            "MultiProcessorCompile"
        }