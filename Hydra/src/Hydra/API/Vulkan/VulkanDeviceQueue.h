#pragma once
#include "Hydra/API/DeviceQueue.h"
#include "Vulkan/vulkan.h"
namespace Hydra
{
	class VulkanDeviceQueue : public DeviceQueue
	{
		friend class VulkanDevice;
	public:
		VulkanDeviceQueue();
		void Create(VkDevice device, uint32_t queueFamily);
	private:
		VkQueue m_Queue;
		uint32_t m_QueueFamilyIndex;
	};
}
