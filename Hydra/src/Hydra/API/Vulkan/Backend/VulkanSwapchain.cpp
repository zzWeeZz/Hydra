#include "HYpch.h"
#include "VulkanSwapchain.h"
#include <GLFW/glfw3.h>
#include "Hydra/Application.h"

namespace Hydra
{
	void VulkanSwapchain::Resize(uint32_t width, uint32_t height)
	{
	}

	void VulkanSwapchain::Create()
	{

	}

	void VulkanSwapchain::Validate()
	{

	}
	void VulkanSwapchain::InternalResize()
	{
	}
	void VulkanSwapchain::CreateSyncObject()
	{
	}
	void VulkanSwapchain::CreateRenderPass()
	{
	}
	void VulkanSwapchain::CreateFrameBuffer()
	{
		for (size_t i = 0; i < m_SwapchainViews.size(); i++)
		{
			VkImageView attachments[] = { m_SwapchainViews[i] };

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_SwapchainRenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = m_SwapchainExtent.width;
			framebufferInfo.height = m_SwapchainExtent.height;
			framebufferInfo.layers = 1;

			//HY_VK_CHECK(vkCreateFramebuffer(GraphicsContext::GetDevice().GetHandle(), &framebufferInfo, nullptr, &m_SwapchainFrameBuffers[i]));
		}
	}
	VkSurfaceFormatKHR VulkanSwapchain::ChooseSwapchainFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}
		return availableFormats[0];
	}
	VkPresentModeKHR VulkanSwapchain::ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}
	VkExtent2D VulkanSwapchain::ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			int32_t width, height;
			auto& glfwWindow = *static_cast<GLFWwindow*>(Application::GetWindow().GetNativeWindow());
			glfwGetFramebufferSize(&glfwWindow, &width, &height);

			VkExtent2D actualExtent = { static_cast<uint32_t>(width),static_cast<uint32_t>(height) };
			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
			return actualExtent;
		}
	}
}