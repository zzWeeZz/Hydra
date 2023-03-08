#include "HYpch.h"
#include "VulkanCommandQueue.h"

namespace Hydra
{
	VulkanCommandQueue::VulkanCommandQueue(Ptr<VulkanDevice> device, Ptr<VulkanDeviceQueue> deviceQueue)
	{
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = deviceQueue.lock()->GetFamilyIndex();
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		HY_VK_CHECK(vkCreateCommandPool(device.lock()->GetHandle(), &poolInfo, nullptr, &m_CommandPool));
	}
	void VulkanCommandQueue::Shutdown(Ptr<VulkanDevice> device)
	{
		vkDestroyCommandPool(device.lock()->GetHandle(), m_CommandPool, nullptr);
	}
	Ref<CommandBuffer> VulkanCommandQueue::AllocateCommandBuffer(Ptr<Device> device, CommandBufferLevel level)
	{
		return ;
	}
}