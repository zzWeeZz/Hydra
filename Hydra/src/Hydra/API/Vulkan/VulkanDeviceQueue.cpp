#include "HYpch.h"
#include "VulkanDeviceQueue.h"

namespace Hydra
{
	VulkanDeviceQueue::VulkanDeviceQueue()
	{
		m_QueueFamilyIndex = ~0; // make it invalid.
		m_Queue = {};
	}
	VulkanDeviceQueue::VulkanDeviceQueue(VkDevice device, uint32_t queueFamily, QueueType type)
	{
		m_Queue = {};
		m_QueueFamilyIndex = queueFamily;
		vkGetDeviceQueue(device, queueFamily, 0, &m_Queue);
		m_QueueType = type;
	}
}