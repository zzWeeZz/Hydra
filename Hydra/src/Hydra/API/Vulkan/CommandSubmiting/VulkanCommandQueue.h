#pragma once
#include "Hydra/API/CommandQueue.h"
#include <Hydra/API/Vulkan/VulkanDeviceQueue.h>
#include <Hydra/API/Vulkan/Backend/VulkanDevice.h>
namespace Hydra
{
	class VulkanCommandQueue : public CommandQueue
	{
	public:
		VulkanCommandQueue() = default;

		void Create(Ptr<VulkanDevice> device, Ptr<VulkanDeviceQueue> queue);

		void Reset() override;
		void Submit(Ptr<Swapchain> swapchain) override;

		[[nodiscard]] FORCEINLINE VkCommandPool GetHandle() { return m_CommandPool; }

		Ref<CommandBuffer> AllocateCommandBuffer(Ptr<Device> device, CommandBufferLevel level) override;

		void Shutdown(Ptr<VulkanDevice> device);

	private:
		VkCommandPool m_CommandPool;
		Ptr<VulkanDevice> m_VulkanDevice;
	
	};
}
