#include "HYpch.h"
#include "VulkanDevice.h"

#include <set>
#include <Hydra/API/Vulkan/VulkanUtils.h>
#include <Hydra/API/Vulkan/CommandSubmiting/VulkanCommandQueue.h>

namespace Hydra
{

	VulkanDevice::VulkanDevice(Ptr<PhysicalDevice> physicalDevice) : Device(physicalDevice)
	{
		
	}
	void VulkanDevice::Create(Ref<VulkanPhysicalDevice> physicalDevice, const std::vector<const char*> validationLayer)
	{
		auto indices = physicalDevice->GetFamilyIndices();
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		

		std::set<uint32_t> uniqueQueueFamilies;

		if (indices.HasGraphics())
		{
			uniqueQueueFamilies.insert(indices.graphicsFamily.value());
		}
		if (indices.HasTranfer())
		{
			uniqueQueueFamilies.insert(indices.transferFamily.value());
		}
		if (indices.HasCompute())
		{
			uniqueQueueFamilies.insert(indices.computeFamily.value());
		}

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};

			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = nullptr;
		createInfo.pNext = nullptr;

		createInfo.enabledExtensionCount = 0;
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayer.size());
		createInfo.ppEnabledLayerNames = validationLayer.data();

		const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		bool extensionsSupported = CheckDeviceExtensionSupport(physicalDevice, deviceExtensions);
	/*	if (!(indices.HasValue() && extensionsSupported && GraphicsContext::SwapchainAdequate()))
		{
			TN_CORE_ASSERT(false, "Device does not support a vulkan swapchain!");
		}*/
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		const VkPhysicalDeviceDynamicRenderingFeatures dynamicRenderingFeature
		{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES,
		.pNext = nullptr,
		.dynamicRendering = VK_TRUE,
		};

		createInfo.pNext = &dynamicRenderingFeature;


		HY_VK_CHECK(vkCreateDevice(physicalDevice->GetHandle(), &createInfo, nullptr, &m_Device));
		HY_CORE_INFO("Vulkan: Successflly created device!");

		if (indices.HasGraphics())
		{
			m_DeviceQueues[QueueType::Graphics] = std::make_shared<VulkanDeviceQueue>(m_Device, indices.graphicsFamily.value(), QueueType::Graphics);
		}
		if (indices.HasTranfer())
		{
			m_DeviceQueues[QueueType::Transfer] = std::make_shared<VulkanDeviceQueue>(m_Device, indices.transferFamily.value(), QueueType::Transfer);
		}
		if (indices.HasCompute())
		{
			m_DeviceQueues[QueueType::Compute] = std::make_shared<VulkanDeviceQueue>(m_Device, indices.computeFamily.value(), QueueType::Compute);
		}

		CreateCommandPools(physicalDevice);
	}
	void VulkanDevice::Shutdown()
	{
		vkDestroyDevice(m_Device, nullptr);
	}
	void VulkanDevice::CreateCommandPools(Ref<VulkanPhysicalDevice> physicalDevice, size_t amount)
	{
		auto indices = physicalDevice->GetFamilyIndices();
		auto& physicalDeviceSpecs = physicalDevice->GetSpecifications();
		if (physicalDeviceSpecs.queueTypes & QueueType::Graphics)
		{
			m_CommandQueues[QueueType::Graphics] = {};
		}
		if (physicalDeviceSpecs.queueTypes & QueueType::Transfer)
		{
			m_CommandQueues[QueueType::Transfer] = {};
		}
		if (physicalDeviceSpecs.queueTypes & QueueType::Compute)
		{
			m_CommandQueues[QueueType::Compute] = {};
		}

		for (auto& [queueType, commandQueues] : m_CommandQueues)
		{
			auto vulkanQueue = std::reinterpret_pointer_cast<VulkanDeviceQueue>(m_DeviceQueues[queueType]);
			for (size_t i = 0; i < g_FramesInFlight; ++i)
			{
				commandQueues[i] = std::make_shared<VulkanCommandQueue>();
				std::reinterpret_pointer_cast<VulkanCommandQueue>(commandQueues[i])->Create(std::reinterpret_pointer_cast<VulkanDevice>(shared_from_this()), vulkanQueue);
			}
		}

	}
	bool VulkanDevice::CheckDeviceExtensionSupport(Ref<VulkanPhysicalDevice> physicalDevice, const std::vector<const char*> deviceExtensions)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice->GetHandle(), nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice->GetHandle(), nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}
}