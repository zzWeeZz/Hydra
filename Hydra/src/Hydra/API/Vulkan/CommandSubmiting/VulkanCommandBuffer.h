#pragma once
#include "Hydra/API/CommandBuffer.h"
#include "Vulkan/vulkan.h"
namespace Hydra
{
	class VulkanCommandBuffer : public CommandBuffer
	{
	public:
		VulkanCommandBuffer(CommandBufferSpecification& specs);

		void Begin() override;
		void End() override;

		void BeginFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer) override;
		void EndFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer) override;


		[[nodiscard]] FORCEINLINE VkCommandBuffer GetHandle() const { return m_CommandBuffer; }
		[[nodiscard]] FORCEINLINE VkCommandBuffer& GetHandle() { return m_CommandBuffer; }
		void Free();
	private:
		VkCommandBuffer m_CommandBuffer;
	};
}
