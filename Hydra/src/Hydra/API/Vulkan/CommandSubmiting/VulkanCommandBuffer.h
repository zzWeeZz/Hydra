#pragma once
#include "Hydra/API/CommandBuffer.h"
#include "Vulkan/vulkan.h"
namespace Hydra
{
	class VulkanCommandBuffer : public CommandBuffer
	{
	public:
		VulkanCommandBuffer(CommandBufferSpecification& specs);
		[[nodiscard]] FORCEINLINE VkCommandBuffer GetHandle() const { return m_CommandBuffer; }
		[[nodiscard]] FORCEINLINE VkCommandBuffer& GetHandle() { return m_CommandBuffer; }
		void Free();
	private:
		VkCommandBuffer m_CommandBuffer;
	};
}
