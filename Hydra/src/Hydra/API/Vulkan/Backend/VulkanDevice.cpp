
#include "VulkanDevice.h"

#include "HYpch.h"
#include <set>
#include <Hydra/API/Vulkan/VulkanUtils.h>

namespace Hydra
{

	VulkanDevice::VulkanDevice(Ptr<PhysicalDevice> physicalDevice) : Device(physicalDevice)
	{
	}
	void VulkanDevice::Create(Ref<VulkanPhysicalDevice> physicalDevice, const std::vector<const char*> validationLayer, VulkanAllocator allocator)
	{
		auto indices = physicalDevice->GetFamilyIndices();
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

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

		vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue);
	}
	void VulkanDevice::CreateCommandPools(Ref<VulkanPhysicalDevice> physicalDevice, VulkanAllocator allocator, size_t amount)
	{
		auto indices = physicalDevice->GetFamilyIndices();
		m_CommandPools.resize(amount);
		m_CommandBuffers.resize(amount);

		for (size_t i = 0; i < amount; ++i)
		{
			VkCommandPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			poolInfo.queueFamilyIndex = indices.graphicsFamily.value();

			HY_VK_CHECK(vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPools[i]));

			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = m_CommandPools[i];
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = 3;

			HY_VK_CHECK(vkAllocateCommandBuffers(m_Device, &allocInfo, m_CommandBuffers[i].data()));
		}
		for (size_t i = 0; i < amount; ++i)
		{
			allocator.QueueDeletion([&, i]() { vkDestroyCommandPool(m_Device, m_CommandPools[i], nullptr); });
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