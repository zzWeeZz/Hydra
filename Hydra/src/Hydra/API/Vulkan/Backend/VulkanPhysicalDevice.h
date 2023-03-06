#pragma once
#include "Hydra/API/PhysicalDevice.h"
#include "Vulkan/vulkan.h"
#include <optional>
namespace Hydra
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		std::optional<uint32_t> transferFamily;

		bool HasValue()
		{
			return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();
		}
	};
	class VulkanPhysicalDevice : public PhysicalDevice
	{
	public:
		VulkanPhysicalDevice(const PhysicalDeviceSpecifications& specs);

		void Create(VkInstance& instance, VkSurfaceKHR& surface);
		QueueFamilyIndices GetFamilyIndices() { return m_QueueFamilyIndices; }
		VkPhysicalDevice GetHandle() { return m_PhysicalDevice; }
	private:
		QueueFamilyIndices FindQueueFamilies(VkSurfaceKHR& surface);
		QueueFamilyIndices m_QueueFamilyIndices;
		VkPhysicalDevice m_PhysicalDevice;
	};
}
