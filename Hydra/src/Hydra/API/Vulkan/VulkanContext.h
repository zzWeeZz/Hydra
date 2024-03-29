#pragma once
#include "Hydra/API/Context.h"
#include "Vulkan/vulkan.h"
#include "Hydra/API/Vulkan/VulkanAllocator.h"
namespace Hydra
{
	struct SwapChainSupportDetails;
	class VulkanContext : public Context
	{
	public:
		// Inherited via Context
		void Initalize(const ContextSpecification& specs) override;
		void Shutdown() override;
		void QuerySwapchainSupport(SwapChainSupportDetails& swapchainDetails);

		void WaitForIdle() override;

		VkSurfaceKHR GetSurface() { return m_Surface; }

	private:
		void CreateSuface();
		void CreateDebugMessanger();
		void DestroyDebugMessanger();
		bool CheckValidationLayerSupport(std::vector<const char*> validationLayer);
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		VkInstance m_Instance{};
		VkSurfaceKHR m_Surface{};
		VkDebugUtilsMessengerEXT m_DebugMessanger{};
	};
}