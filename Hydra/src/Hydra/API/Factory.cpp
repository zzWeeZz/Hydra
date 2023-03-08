#include "HYpch.h"
#include "Factory.h"
#include <Hydra/API/DX12/Backend/DxPhysicalDevice.h>
#include <Hydra/API/DX12/Backend/DxDevice.h>
#include <Hydra/API/DX12/Backend/DxSwapchain.h>
#include <Hydra/API/DX12/DxContext.h>
#include <Hydra/API/Vulkan/VulkanContext.h>
namespace Hydra
{
	void Factory::ContructContext(ContextConstructSpecifications& specs)
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

	}
}