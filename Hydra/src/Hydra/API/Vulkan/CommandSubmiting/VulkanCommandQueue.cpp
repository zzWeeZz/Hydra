#include "HYpch.h"
#include "VulkanCommandQueue.h"
#include <Hydra/API/Vulkan/CommandSubmiting/VulkanCommandBuffer.h>

namespace Hydra
{
	
	void VulkanCommandQueue::Shutdown(Ptr<VulkanDevice> device)
	{
		vkDestroyCommandPool(device.lock()->GetHandle(), m_CommandPool, nullptr);
	}
	void VulkanCommandQueue::Create(Ptr<VulkanDevice> device, Ptr<VulkanDeviceQueue> queue)
	{
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queue.lock()->GetFamilyIndex();
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		HY_VK_CHECK(vkCreateCommandPool(device.lock()->GetHandle(), &poolInfo, nullptr, &m_CommandPool));
		m_CommandBuffer = AllocateCommandBuffer(device, CommandBufferLevel::Primary);
		VulkanAllocator::CustomDeletion([this, device]() { Shutdown(device); });
	}
	Ref<CommandBuffer> VulkanCommandQueue::AllocateCommandBuffer(Ptr<Device> device, CommandBufferLevel level)
	{
		auto shr = shared_from_this();
		CommandBufferSpecification cmdSpecs = {};
		cmdSpecs.device = device;
		cmdSpecs.queue = std::reinterpret_pointer_cast<VulkanCommandQueue>(shared_from_this());
		cmdSpecs.level = level;
		return std::make_shared<VulkanCommandBuffer>(cmdSpecs);
	}
}