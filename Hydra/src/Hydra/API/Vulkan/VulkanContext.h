#pragma once
#include "Hydra/API/Context.h"
#include "Vulkan/vulkan.h"
namespace Hydra
{
	class VulkanContext : public Context
	{
		friend class VulkanAllocator;
	public:
		// Inherited via Context
		void Initalize(const GraphicsContextSpecification& specs) override;
	private:
		bool CheckValidationLayerSupport(std::vector<const char*> validationLayer);
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		VkInstance m_Instance;
		VkSurfaceKHR m_Surface;
		VkDebugUtilsMessengerEXT m_DebugMessager;
		VulkanAllocator m_Allocator;
	};
}