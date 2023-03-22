#pragma once
#include "Hydra/API/CommandBuffer.h"
#include "Vulkan/vulkan.h"
#include "Hydra/API/Vulkan/Descriptors/DescriptorLayoutCache.h"
#include "Hydra/API/Vulkan/Descriptors/DescriptorAllocator.h"
namespace Hydra
{
	class VulkanGraphicsPipeline;

	class VulkanCommandBuffer : public CommandBuffer
	{
	public:
		VulkanCommandBuffer(CommandBufferSpecification& specs);

		void Begin() override;
		void End() override;

		void BeginFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer, float color[4]) override;
		void EndFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer) override;

		void DrawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override;
		void DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) override;

		void BindGraphicsPipeline(uint32_t frameIndex, Ref<GraphicsPipeline>& pipeline) override;

		void BindVertexBuffer(uint32_t frameindex, Ref<Buffer>& buffer) override;
		void BindIndexBuffer(uint32_t frameindex, Ref<Buffer>& buffer) override;
		void BindConstantBuffer(uint32_t frameindex, uint32_t bindPoint, uint32_t space, Ref<Buffer>& buffer) override;

		void CopyFramebufferToSwapchain(uint32_t frameIndex, Ref<Framebuffer>& framebuffer, Ref<Swapchain> swapchain) override;

		[[nodiscard]] FORCEINLINE VkCommandBuffer GetHandle() const { return m_CommandBuffer; }
		[[nodiscard]] FORCEINLINE VkCommandBuffer& GetHandle() { return m_CommandBuffer; }
		void Free();
	private:
		VkCommandBuffer m_CommandBuffer;
		DescriptorLayoutCache m_DescriptorCache;
		DescriptorAllocator m_DescriptorAllocator;
		Ptr<VulkanGraphicsPipeline> m_CurrentPipeline;

		PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
	};
}
