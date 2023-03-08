#pragma once
#include "Hydra/API/PhysicalDevice.h"
#include "Vulkan/vulkan.h"
#include <optional>
namespace Hydra
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> transferFamily;
		std::optional<uint32_t> computeFamily;

		inline bool HasAllValue()
		{
			return graphicsFamily.has_value() && transferFamily.has_value() && computeFamily.has_value();
		}

		inline bool HasGraphics()
		{
			return graphicsFamily.has_value();
		}

		inline bool HasTranfer()
		{
			return transferFamily.has_value();
		}

		inline bool HasCompute()
		{
			return computeFamily.has_value();
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
