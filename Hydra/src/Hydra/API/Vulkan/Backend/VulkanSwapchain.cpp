#include "HYpch.h"
#include "VulkanSwapchain.h"
#include <GLFW/glfw3.h>
#include "Hydra/Application.h"
#include <Hydra/API/Vulkan/Backend/VulkanPhysicalDevice.h>
#include <Hydra/API/Vulkan/VulkanContext.h>
#include <Hydra/API/Vulkan/Backend/VulkanDevice.h>

namespace Hydra
{
	VulkanSwapchain::VulkanSwapchain(const SwapchainSpecfications& specs) : Swapchain(specs)
	{
	}
	void VulkanSwapchain::Resize(uint32_t width, uint32_t height)
	{
	}

	void VulkanSwapchain::Create(Ptr<Context> context)
	{
		Validate(context);
		CreateSyncObject();
		CreateRenderPass();
		m_SwapchainFrameBuffers.resize(m_SwapchainViews.size());
		CreateFrameBuffer();
		HY_CORE_INFO("Vulkan: Successfully created Swapchain!");
	}

	void VulkanSwapchain::Validate(Ptr<Context> context)
	{
		auto vulkanContext = std::reinterpret_pointer_cast<VulkanContext>(context.lock());
		auto vulkanDevice = std::reinterpret_pointer_cast<VulkanDevice>(vulkanContext->GetDevice().lock());
		auto vulkanPhysicalDevice = std::reinterpret_pointer_cast<VulkanPhysicalDevice>(vulkanContext->GetPhyicalDevice().lock());

		SwapChainSupportDetails supportDetails;
		vulkanContext->QuerySwapchainSupport(supportDetails);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapchainFormat(supportDetails.formats);
		VkPresentModeKHR presentMode = ChooseSwapchainPresentMode(supportDetails.presentModes);

		VkExtent2D extent = ChooseSwapchainExtent(supportDetails.capabilities);

		m_SwapchainExtent = extent;
		m_SwapchainFormat = surfaceFormat.format;

		uint32_t imageCount = supportDetails.capabilities.minImageCount + 1;
		if (supportDetails.capabilities.maxImageCount > 0 && imageCount > supportDetails.capabilities.maxImageCount)
		{
			imageCount = supportDetails.capabilities.maxImageCount;
		}
		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = vulkanContext->GetSurface();

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = m_SwapchainExtent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional

		createInfo.preTransform = supportDetails.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;
		HY_VK_CHECK(vkCreateSwapchainKHR(vulkanDevice->GetHandle(), &createInfo, nullptr, &m_Swapchain));
		vkGetSwapchainImagesKHR(vulkanDevice->GetHandle(), m_Swapchain, &imageCount, nullptr);
		m_SwapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(vulkanDevice->GetHandle(), m_Swapchain, &imageCount, m_SwapchainImages.data());


		m_SwapchainViews.resize(m_SwapchainImages.size());
		for (size_t i = 0; i < m_SwapchainViews.size(); ++i)
		{
			VkImageViewCreateInfo viewCreateInfo{};
			viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewCreateInfo.image = m_SwapchainImages[i];

			viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewCreateInfo.format = m_SwapchainFormat;

			viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewCreateInfo.subresourceRange.baseMipLevel = 0;
			viewCreateInfo.subresourceRange.levelCount = 1;
			viewCreateInfo.subresourceRange.baseArrayLayer = 0;
			viewCreateInfo.subresourceRange.layerCount = 1;

			HY_VK_CHECK(vkCreateImageView(vulkanDevice->GetHandle(), &viewCreateInfo, nullptr, &m_SwapchainViews[i]));
		}
		HY_CORE_INFO("Vulkan: Successfully created swapchain images and views!");
	}
	void VulkanSwapchain::Present()
	{
		auto vulkanDevice = std::reinterpret_pointer_cast<VulkanDevice>(m_Specs.context.lock()->GetDevice().lock());
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		//presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_Swapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		//presentInfo.pImageIndices = &imageIndex;
		auto vulkanQueue = std::reinterpret_pointer_cast<VulkanDeviceQueue>(vulkanDevice->GetQueue(QueueType::Graphics).lock());
		vkQueuePresentKHR(vulkanQueue->GetHandle(), &presentInfo);
	}
	void VulkanSwapchain::CleanUp()
	{
		auto vulkanDevice = std::reinterpret_pointer_cast<VulkanDevice>(m_Specs.context.lock()->GetDevice().lock());
		auto device = vulkanDevice->GetHandle();
		for (auto& SwapchainFrameBuffer : m_SwapchainFrameBuffers)
		{
			vkDestroyFramebuffer(device, SwapchainFrameBuffer, nullptr);
		}
		for (auto& SwapchainView : m_SwapchainViews)
		{
			vkDestroyImageView(device, SwapchainView, nullptr);
		}
		vkDestroySwapchainKHR(device, m_Swapchain, nullptr);
	}
	void VulkanSwapchain::Shutdown(bool destroyRenderTarget)
	{
		auto vulkanDevice = std::reinterpret_pointer_cast<VulkanDevice>(m_Specs.context.lock()->GetDevice().lock());
		auto device = vulkanDevice->GetHandle();

		if (destroyRenderTarget) vkDestroyRenderPass(device, m_SwapchainRenderPass, nullptr);

		for (size_t i = 0; i < g_FramesInFlight; ++i)
		{
			vkDestroySemaphore(device, m_ImageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(device, m_RenderFinishedSemaphores[i], nullptr);
			vkDestroyFence(device, m_InFlightFences[i], nullptr);
		}
	}
	void VulkanSwapchain::InternalResize()
	{
	}
	void VulkanSwapchain::CreateSyncObject()
	{
		auto vulkanDevice = std::reinterpret_pointer_cast<VulkanDevice>(m_Specs.context.lock()->GetDevice().lock());
		auto device = vulkanDevice->GetHandle();
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		// Creates semaphore and fence per frame-in-flight.
		for (size_t i = 0; i < g_FramesInFlight; ++i)
		{
			HY_VK_CHECK(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]));
			HY_VK_CHECK(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]));
			HY_VK_CHECK(vkCreateFence(device, &fenceInfo, nullptr, &m_InFlightFences[i]));
		}
		HY_CORE_INFO("Vulkan: Successfully created swapchain syncObjects!");
	}
	void VulkanSwapchain::CreateRenderPass()
	{
		auto vulkanDevice = std::reinterpret_pointer_cast<VulkanDevice>(m_Specs.context.lock()->GetDevice().lock());
		auto device = vulkanDevice->GetHandle();
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = m_SwapchainFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;


		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		HY_VK_CHECK(vkCreateRenderPass(device, &renderPassInfo, nullptr, &m_SwapchainRenderPass));
		HY_CORE_INFO("Vulkan: Successfully created swapchain Renderpass!");
	}
	void VulkanSwapchain::CreateFrameBuffer()
	{
		auto vulkanDevice = std::reinterpret_pointer_cast<VulkanDevice>(m_Specs.context.lock()->GetDevice().lock());
		auto device = vulkanDevice->GetHandle();
		size_t i = 0;
		for (auto& SwapchainView : m_SwapchainViews)
		{
			VkImageView attachments[] = { SwapchainView };

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_SwapchainRenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = m_SwapchainExtent.width;
			framebufferInfo.height = m_SwapchainExtent.height;
			framebufferInfo.layers = 1;

			HY_VK_CHECK(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &m_SwapchainFrameBuffers[i]));
			i++;
		}
		HY_CORE_INFO("Vulkan: Successfully created swapchain framebuffer!");
	}
	VkSurfaceFormatKHR VulkanSwapchain::ChooseSwapchainFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				HY_CORE_INFO("Vulkan: Found the right swapchain format!");
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
				HY_CORE_INFO("Vulkan: Found the right present moved!");
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