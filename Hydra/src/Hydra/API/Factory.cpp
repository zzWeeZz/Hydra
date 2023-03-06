#include "HYpch.h"
#include "Factory.h"
#include <Hydra/API/DX12/Backend/DxPhysicalDevice.h>
#include <Hydra/API/DX12/Backend/DxDevice.h>
#include <Hydra/API/DX12/Backend/DxSwapchain.h>
namespace Hydra
{
	void Factory::BuildBackend(Ref<PhysicalDevice> physicalDevice, Ref<Device> device, Ref<Swapchain> swapchain, API api)
	{
		switch (api)
		{
		case Hydra::API::Dx12:
			BuildDx12Backend(physicalDevice, device, swapchain);
			break;
		case Hydra::API::Vulkan:
			BuildVulkanBackend(physicalDevice, device, swapchain);
			break;
		default:
			HY_CORE_ERROR("Factory: Failed to find api in system!");
			break;
		}
	}
	void Factory::BuildVulkanBackend(Ref<PhysicalDevice> physicalDevice, Ref<Device> device, Ref<Swapchain> swapchain)
	{
		
	}
	void Factory::BuildDx12Backend(Ref<PhysicalDevice> physicalDevice, Ref<Device> device, Ref<Swapchain> swapchain)
	{
		PhysicalDeviceSpecifications pdSpecs{};
		physicalDevice = (std::make_shared<DxPhysicalDevice>(pdSpecs));
		device = (std::make_shared<DxDevice>(physicalDevice));
		SwapchainSpecfications specs{};
		specs.frameCount = 3;
		specs.device = device;
		specs.physicalDevice = physicalDevice;
		swapchain = (std::make_shared<DxSwapchain>(specs));
	}
}