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
		VulkanDeviceQueue(VkDevice device, uint32_t queueFamily, QueueType type);

		[[nodiscard]] FORCEINLINE VkQueue GetHandle() { return m_Queue; };
		[[nodiscard]] FORCEINLINE uint32_t GetFamilyIndex() const { return m_QueueFamilyIndex; }
	private:
		VkQueue m_Queue;
		uint32_t m_QueueFamilyIndex;
	};
}
