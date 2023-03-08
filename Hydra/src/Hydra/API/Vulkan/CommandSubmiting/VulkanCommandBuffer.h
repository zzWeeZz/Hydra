#pragma once
#include "Hydra/API/CommandBuffer.h"
#include "Vulkan/vulkan.h"
namespace Hydra
{
	class VulkanCommandBuffer : public CommandBuffer
	{
	public:
		VulkanCommandBuffer(CommandBufferSpecification& specs);
		void Free();
	private:
		VkCommandBuffer m_CommandBuffer;
	};
}
