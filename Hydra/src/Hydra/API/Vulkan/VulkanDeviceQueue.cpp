#include "HYpch.h"
#include "VulkanDeviceQueue.h"

namespace Hydra
{
	VulkanDeviceQueue::VulkanDeviceQueue()
	{
	}
	void VulkanDeviceQueue::Create(VkDevice device, uint32_t queueFamily)
	{
		m_QueueFamilyIndex = queueFamily;
		vkGetDeviceQueue(device, queueFamily, 0, &m_Queue);
	}
}