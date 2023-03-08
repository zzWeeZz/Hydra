#include "HYpch.h"
#include "HYpch.h"
#include "VulkanPhysicalDevice.h"

namespace Hydra
{

	VulkanPhysicalDevice::VulkanPhysicalDevice(const PhysicalDeviceSpecifications& specs) : PhysicalDevice(specs)
	{
	}

	void VulkanPhysicalDevice::Create(VkInstance& instance, VkSurfaceKHR& surface)
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0)
		{
			HY_CORE_ERROR("Failed to find GPU with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		// find a device that is suitable
		std::vector<std::pair<uint32_t, VkPhysicalDevice>> candidates;
		for (const auto& device : devices)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
			uint32_t score = 0;
			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				score += 1000;
			}

			score += deviceProperties.limits.maxImageDimension2D;

			if (!deviceFeatures.geometryShader)
			{
				score = 0;
			}

			candidates.emplace_back(score, device);
		}
		if (candidates.rbegin()->first > 0)
		{
			HY_CORE_INFO("Vulkan: Physical Device found!");
			m_PhysicalDevice = candidates.rbegin()->second;
			VkPhysicalDeviceMeshShaderPropertiesNV meshShaderProperties{};
			meshShaderProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_PROPERTIES_NV;
			VkPhysicalDeviceProperties2 deviceProperties = {};
			deviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
			deviceProperties.pNext = &meshShaderProperties;
			vkGetPhysicalDeviceProperties2(m_PhysicalDevice, &deviceProperties);
		}

		m_QueueFamilyIndices = FindQueueFamilies(surface);
	}
	QueueFamilyIndices VulkanPhysicalDevice::FindQueueFamilies(VkSurfaceKHR& surface)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

		int32_t i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (m_Specs.queueTypes & QueueType::Graphics)
			{
				if (indices.HasGraphics() == false)
				{
					if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
					{
						indices.graphicsFamily = i;
						VkBool32 presentSupport = false;
						vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, surface, &presentSupport);
					}
				}
			}

			if (m_Specs.queueTypes & QueueType::Transfer)
			{
				if (indices.HasTranfer() == false)
				{
					if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
					{
						indices.transferFamily = i;
					}
				}
			}

			if (m_Specs.queueTypes & QueueType::Compute)
			{
				if (indices.HasCompute() == false)
				{
					if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
					{
						indices.computeFamily = i;
					}
				}
			}
			i++;
		}


		return indices;
	}
}
