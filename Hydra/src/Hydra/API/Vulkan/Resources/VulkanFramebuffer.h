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
	private:

		// Inherited via Framebuffer

		void Validate();

		VkRect2D m_Rect;
		PerFrameInFlight<std::vector<VkRenderingAttachmentInfo>> m_Attachments;
		PerFrameInFlight<std::vector<AllocatedImage>> m_Images;
		PerFrameInFlight<std::vector<VkImageView>> m_Views;
	};
}
