#pragma once
#include "Hydra/API/Device.h"
#include "Vulkan/vulkan.h"
#include "Hydra/API/PhysicalDevice.h"
#include <Hydra/API/Vulkan/Backend/VulkanPhysicalDevice.h>
#include <vector>
#include <Hydra/API/Vulkan/VulkanAllocator.h>
namespace Hydra
{
	class VulkanDevice : public Device
	{
	public:
		VulkanDevice(Ptr<PhysicalDevice> physicalDevice);

		void Create(Ref<VulkanPhysicalDevice> physicalDevice, const std::vector<const char*> validationLayer, VulkanAllocator allocator);

	private:
		void CreateCommandPools(Ref<VulkanPhysicalDevice> physicalDevice, VulkanAllocator allocator, size_t amount = 1);
		bool CheckDeviceExtensionSupport(Ref<VulkanPhysicalDevice> physicalDevice, const std::vector<const char*> deviceExtensions);

		VkDevice m_Device;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		std::vector<PerFrameInFlight<VkCommandBuffer>> m_CommandBuffers;
		std::vector<VkCommandPool> m_CommandPools;
	};
}