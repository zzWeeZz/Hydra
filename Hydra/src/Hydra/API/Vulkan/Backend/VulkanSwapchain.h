#pragma once
#include "Hydra/API/Swapchain.h"

#include "Vulkan/vulkan.h"

#include "Hydra/API/Vulkan/VulkanUtils.h"

namespace Hydra
{
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
	class Context;
	class VulkanSwapchain : public Swapchain
	{
	public:
		VulkanSwapchain(const SwapchainSpecfications& specs);
		void Resize(uint32_t width, uint32_t height) override;
		void Create(Ptr<Context> context) override;
		void Validate(Ptr<Context> context) override;
		uint32_t PrepareNewFrame() override;

		[[nodiscard]] FORCEINLINE VkSemaphore& GetImageAvailableSemaphore() { return m_ImageAvailableSemaphores[m_CurrentImage]; }
		[[nodiscard]] FORCEINLINE VkSemaphore& GetRenderFinishedSemaphore() { return m_RenderFinishedSemaphores[m_CurrentFrame]; }
		[[nodiscard]] FORCEINLINE VkFence& GetInFlightFence() { return m_InFlightFences[m_CurrentFrame]; }

		void Present() override;
		void CleanUp();
		void Shutdown(bool destroyRenderTarget);
	protected:
	
	private:

		void GetCurrentImageIndex();
		
		// Resizes the swapchain after width and height.
		// 
		// HOW IT WORKS:
		// Cleans up the swapchain by deallocating swapchain and its images.
		// Calls Validate to recreate the swapchain.
		void InternalResize();
		void CreateSyncObject();

		// Creates a basic VkRenderpass connected to the swapchain.
		// OBS: This renderpass is only used by ImGui!
		void CreateRenderPass();

		// Creates basic Vkframebuffers per swapchain image.
		// OBS: These Vkframebuffers is only used by ImGui!
		void CreateFrameBuffer();

		// Tires to get teh optimal swapchain format for the current gpu.
		VkSurfaceFormatKHR ChooseSwapchainFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		// Tries to get the optimal present mode for the current gpu.
		VkPresentModeKHR ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		// Builds a vkExtent after the screen size.
		VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		VkSwapchainKHR m_Swapchain;
		std::vector<VkImage> m_SwapchainImages;
		std::vector<VkImageView> m_SwapchainViews;
		VkFormat m_SwapchainFormat;
		VkExtent2D m_SwapchainExtent;

		VkRenderPass m_SwapchainRenderPass;
		std::vector<VkFramebuffer> m_SwapchainFrameBuffers;
		PerFrameInFlight<VkSemaphore> m_ImageAvailableSemaphores;
		PerFrameInFlight<VkSemaphore> m_RenderFinishedSemaphores;
		PerFrameInFlight<VkFence> m_InFlightFences;

		// Inherited via Swapchain
	};
}