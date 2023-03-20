#pragma once
#include "Hydra/API/Framebuffer.h"
#include "Hydra/API/Vulkan/VulkanUtils.h"
namespace Hydra
{
	class VulkanFramebuffer : public Framebuffer
	{
		friend class VulkanCommandBuffer;
	public:
		VulkanFramebuffer(FramebufferSpecification& specs, Ptr<Device> device);
		void CleanUp();

		[[nodiscard]] FORCEINLINE VkRect2D& GetRect() { return m_Rect; }

	private:

		// Inherited via Framebuffer

		void Validate();

		VkRect2D m_Rect;
		PerFrameInFlight<std::vector<VkRenderingAttachmentInfo>> m_Attachments;
		PerFrameInFlight<std::vector<AllocatedImage>> m_Images;
		PerFrameInFlight<std::vector<VkImageView>> m_Views;
	};
}
