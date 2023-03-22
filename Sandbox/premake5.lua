project "SandBox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    disablewarnings { "4996", "6285", "26437", "26451", "26498", "26800", "26495", "26439" }
    debugdir "../AssetData/"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")
    files
	{
		"src/**.cpp",
        "src/**.h"
	}

    includedirs
    {
        "src/",
        "../Hydra/src",
        IncludeDir["GLFW"],
        IncludeDir["spdlog"],
        IncludeDir["VulkanSDK"],
        IncludeDir["Dx12Helpers"],
    }
    libdirs
    {
    }
    links
    {
        "Hydra",
        "GLFW",
        LibDir["Vulkan"],
        LibDir["DxC"],
        LibDir["Dx12"],
        LibDir["DXGI"],
        LibDir["SPIRVReflect"],
        LibDir["glm"]
    }
    linkoptions 
	{
		"/ignore:4006",
		"/ignore:4099"
	}

    defines {"_CONSOLE", "GLM_FORCE_DEPTH_ZERO_TO_ONE"}

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
        defines
        {
            "HY_CONFIG_DEBUG"
        }
        links
        {
            VulkanDebugLibs
        }
      

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
        defines
        {
            "HY_CONFIG_RELEASE"
        }
        links
        {
            VulkanReleaseLibs
        }

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"
        defines
        {
            "HY_CONFIG_DIST"
        }
        links
        {
            VulkanReleaseLibs
        }
    
    filter "system:windows"
		symbols "On"		
		systemversion "latest"

		flags 
        { 
			"MultiProcessorCompile"
		}