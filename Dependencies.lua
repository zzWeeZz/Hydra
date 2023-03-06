VULKAN_SDK = os.getenv("VULKAN_SDK")

FileDir = {};
FileDir["VMA"] = "%{wks.location}/Hydra/vendor/vma/vk_mem_alloc.h"

IncludeDir = {};

IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include/"
IncludeDir["GLFW"] = "%{wks.location}/Hydra/vendor/GLFW/include"
IncludeDir["spdlog"] = "%{wks.location}/Hydra/vendor/spdlog/include"
IncludeDir["Dx12Helpers"] = "%{wks.location}/Hydra/vendor/dx12helpers"
IncludeDir["Vendor"] = "%{wks.location}/Hydra/vendor/"

LibDir = {};

LibDir["Vulkan"] = "%{VULKAN_SDK}/Lib/vulkan-1.lib"
LibDir["DxC"] = "%{VULKAN_SDK}/Lib/dxcompiler.lib"
LibDir["Dx12"] = "D3d12.lib"
LibDir["DXGI"] = "DXGI.lib"

VulkanDebugLibs = 
{
    "%{VULKAN_SDK}/Lib/GenericCodeGend.lib",
    "%{VULKAN_SDK}/Lib/glslangd.lib",
    "%{VULKAN_SDK}/Lib/glslang-default-resource-limitsd.lib",
    "%{VULKAN_SDK}/Lib/HLSLd.lib",
    "%{VULKAN_SDK}/Lib/MachineIndependentd.lib",
    "%{VULKAN_SDK}/Lib/OGLCompilerd.lib",
    "%{VULKAN_SDK}/Lib/OSDependentd.lib",
    "%{VULKAN_SDK}/Lib/shaderc_combinedd.lib",
    "%{VULKAN_SDK}/Lib/shaderc.lib",
    "%{VULKAN_SDK}/Lib/shaderc_sharedd.lib",
    "%{VULKAN_SDK}/Lib/shaderc_utild.lib",
    "%{VULKAN_SDK}/Lib/shadercd.lib",
    "%{VULKAN_SDK}/Lib/SPIRVd.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-cd.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-cored.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-cppd.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-c-sharedd.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-glsld.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-hlsld.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-msld.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-reflectd.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-utild.lib",
    "%{VULKAN_SDK}/Lib/SPIRV-Toolsd.lib",
    "%{VULKAN_SDK}/Lib/SPIRV-Tools-diffd.lib",
    "%{VULKAN_SDK}/Lib/SPIRV-Tools-linkd.lib",
    "%{VULKAN_SDK}/Lib/SPIRV-Tools-optd.lib",
    "%{VULKAN_SDK}/Lib/SPIRV-Tools-reduced.lib",
    "%{VULKAN_SDK}/Lib/SPIRV-Tools-sharedd.lib",
    "%{VULKAN_SDK}/Lib/SPVRemapperd.lib",
    "%{VULKAN_SDK}/Lib/VkLayer_utils.lib",
}

VulkanReleaseLibs = 
{
    "%{VULKAN_SDK}/Lib/GenericCodeGen.lib",
    "%{VULKAN_SDK}/Lib/glslang.lib",
    "%{VULKAN_SDK}/Lib/glslang-default-resource-limits.lib",
    "%{VULKAN_SDK}/Lib/HLSL.lib",
    "%{VULKAN_SDK}/Lib/MachineIndependent.lib",
    "%{VULKAN_SDK}/Lib/OGLCompiler.lib",
    "%{VULKAN_SDK}/Lib/OSDependent.lib",
    "%{VULKAN_SDK}/Lib/shaderc_combined.lib",
    "%{VULKAN_SDK}/Lib/shaderc.lib",
    "%{VULKAN_SDK}/Lib/shaderc_shared.lib",
    "%{VULKAN_SDK}/Lib/shaderc_util.lib",
    "%{VULKAN_SDK}/Lib/shaderc.lib",
    "%{VULKAN_SDK}/Lib/SPIRV.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-c.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-core.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-cpp.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-c-shared.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-glsl.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-hlsl.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-msl.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-reflect.lib",
    "%{VULKAN_SDK}/Lib/spirv-cross-util.lib",
    "%{VULKAN_SDK}/Lib/SPIRV-Tools.lib",
    "%{VULKAN_SDK}/Lib/SPIRV-Tools-diff.lib",
    "%{VULKAN_SDK}/Lib/SPIRV-Tools-link.lib",
    "%{VULKAN_SDK}/Lib/SPIRV-Tools-opt.lib",
    "%{VULKAN_SDK}/Lib/SPIRV-Tools-reduce.lib",
    "%{VULKAN_SDK}/Lib/SPIRV-Tools-shared.lib",
    "%{VULKAN_SDK}/Lib/SPVRemapper.lib",
    "%{VULKAN_SDK}/Lib/VkLayer_utils.lib",
}