#pragma once
#include "Hydra/API/PhysicalDevice.h"
#include "Hydra/API/Device.h"
#include "Hydra/API/Swapchain.h"

namespace Hydra
{
	enum class API
	{
		Dx12,
		Vulkan
	};
	class Factory
	{
	public:
		static void BuildBackend(Ref<PhysicalDevice> physicalDevice, Ref<Device> device, Ref<Swapchain> swapchain, API api);
	private:
		static void BuildVulkanBackend(Ref<PhysicalDevice> physicalDevice, Ref<Device> device, Ref<Swapchain> swapchain);
		static void BuildDx12Backend(Ref<PhysicalDevice> physicalDevice, Ref<Device> device, Ref<Swapchain> swapchain);
	};
}