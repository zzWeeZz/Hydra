#include "HYpch.h"
#include "VulkanCommandQueue.h"
#include <Hydra/API/Vulkan/CommandSubmiting/VulkanCommandBuffer.h>
#include "Hydra/API/Vulkan/Backend/VulkanSwapchain.h"
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
		m_VulkanDevice = device;
		m_Queue = queue;
		VulkanAllocator::CustomDeletion([this, device]() { Shutdown(device); });
	}

	void VulkanCommandQueue::Reset()
	{
		if (m_VulkanDevice.expired() == false)
		{
			vkResetCommandPool(m_VulkanDevice.lock()->GetHandle(), m_CommandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
		}
	}

	void VulkanCommandQueue::Submit(Ptr<Swapchain> swapchain)
	{
		if (m_Queue.expired())
		{
			return;
		}

		auto vulkanCommandBuffer = std::reinterpret_pointer_cast<VulkanCommandBuffer>(m_CommandBuffer);
		auto vulkanSwapchain = std::reinterpret_pointer_cast<VulkanSwapchain>(swapchain.lock());
		auto vulkanGraphicsQueue = std::reinterpret_pointer_cast<VulkanDeviceQueue>(m_Queue.lock());
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore signalSemaphores[] = { vulkanSwapchain->GetRenderFinishedSemaphore() };
		VkSemaphore waitSemaphores[] = { vulkanSwapchain->GetImageAvailableSemaphore() };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &vulkanCommandBuffer->GetHandle();

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;
		HY_VK_CHECK(vkQueueSubmit(vulkanGraphicsQueue->GetHandle(), 1, &submitInfo, vulkanSwapchain->GetInFlightFence()));
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