#include "HYpch.h"
#include "Factory.h"
#include <Hydra/API/DX12/Backend/DxPhysicalDevice.h>
#include <Hydra/API/DX12/Backend/DxDevice.h>
#include <Hydra/API/DX12/Backend/DxSwapchain.h>
#include <Hydra/API/DX12/DxContext.h>
#include <Hydra/API/Vulkan/VulkanContext.h>
#include <Hydra/API/Vulkan/VulkanShaderCompiler.h>
#include <Hydra/API/DX12/DxShaderCompiler.h>
namespace Hydra
{
	void Factory::ConstructContext(ContextConstructSpecifications& specs)
	{
		HY_CORE_ASSERT((specs.contextSpecs.queueTypes & QueueType::Graphics), "Factory: In order to construct a context you need to have Queue type: [Graphics]!");

		
		switch (specs.api)
		{
		case Hydra::API::Dx12:
			specs.context = std::make_shared<DxContext>();
			break;
		case Hydra::API::Vulkan:
			specs.context = std::make_shared<VulkanContext>();
			break;
		default:
			HY_CORE_ERROR("Factory: Failed to find api in system!");
			break;
		}
		specs.context->Initalize(specs.contextSpecs);
		s_API = specs.api;
		s_ContextHandle = specs.context;

	}
	void Factory::ConstructShaderCompiler(ShaderCompilerCreateSpecification& specs, Ref<ShaderCompiler>& outShaderCompiler)
	{
		switch (s_API)
		{
		case Hydra::API::Dx12:
			outShaderCompiler = std::make_shared<DxShaderCompiler>(specs);
			break;
		case Hydra::API::Vulkan:
			outShaderCompiler = std::make_shared<VulkanShaderCompiler>(specs);
			break;
		default:
			HY_CORE_ERROR("Factory: Failed to find api in system!");
			break;
		}
	}
}