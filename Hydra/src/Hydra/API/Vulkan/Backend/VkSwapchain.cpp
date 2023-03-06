#include "HYpch.h"
#include "VkSwapchain.h"

namespace Hydra
{
	void VkSwapchain::Resize(uint32_t width, uint32_t height)
	{
		Validate();
	}

	void VkSwapchain::Create()
	{

	}

	void VkSwapchain::Validate()
	{
	}
	void VkSwapchain::InternalResize()
	{
	}
	void VkSwapchain::CreateSyncObject()
	{
	}
	void VkSwapchain::CreateRenderPass()
	{
	}
	void VkSwapchain::CreateFrameBuffer()
	{
	}
	VkSurfaceFormatKHR VkSwapchain::ChooseSwapchainFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		return {};
	}
	VkPresentModeKHR VkSwapchain::ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		return VkPresentModeKHR();
	}
	VkExtent2D VkSwapchain::ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		return {};
	}
}