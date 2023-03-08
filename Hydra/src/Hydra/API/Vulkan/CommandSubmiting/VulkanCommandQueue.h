#pragma once
#include "Hydra/API/CommandQueue.h"
#include <Hydra/API/Vulkan/VulkanDeviceQueue.h>
#include <Hydra/API/Vulkan/Backend/VulkanDevice.h>
namespace Hydra
{
	class VulkanCommandQueue : public CommandQueue
	{
	public:
		VulkanCommandQueue(Ptr<VulkanDevice> device, Ptr<VulkanDeviceQueue> deviceQueue);

		[[nodiscard]] FORCEINLINE VkCommandPool GetHandle() { return m_CommandPool; }

		Ref<CommandBuffer> AllocateCommandBuffer(Ptr<Device> device, CommandBufferLevel level) override;

		void Shutdown(Ptr<VulkanDevice> device);

	private:
		VkCommandPool m_CommandPool;

		// Inherited via CommandQueue
	};
}
