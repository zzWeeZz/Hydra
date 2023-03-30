#include "HYpch.h"
#include "VulkanDevice.h"

#include <set>

#include <Hydra/API/Vulkan/VulkanUtils.h>

#include <Hydra/API/Vulkan/CommandSubmiting/VulkanCommandQueue.h>

#include "Hydra/API/Vulkan/Resources/VulkanBuffer.h"
#include "Hydra/API/Vulkan/Resources/VulkanFramebuffer.h"

#include "Hydra/API/Vulkan/Pipeline/VulkanGraphicsPipeline.h"

#include "Hydra/API/Vulkan/Resources/VulkanImage.h"

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

		const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_push_descriptor"};
		bool extensionsSupported = CheckDeviceExtensionSupport(physicalDevice, deviceExtensions);
		if (!(extensionsSupported))
		{
			HY_CORE_ASSERT(false, "Device does not support a vulkan swapchain!");
		}
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
		m_SamplerLib.Initalize(std::reinterpret_pointer_cast<VulkanDevice>(shared_from_this()));
		m_SamplerLib.Add("2", VulkanFilter::Nearest, VulkanAddress::Repeat, VulkanMipmapMode::Nearest, false);
		m_SamplerLib.Add("3", VulkanFilter::Nearest, VulkanAddress::ClampToBorder, VulkanMipmapMode::Nearest, false);
		m_SamplerLib.Add("4", VulkanFilter::Linear, VulkanAddress::Repeat, VulkanMipmapMode::Linear, false);
		m_SamplerLib.Add("5", VulkanFilter::Linear, VulkanAddress::ClampToBorder, VulkanMipmapMode::Linear, false);
		//m_SamplerLib.Add("4", VulkanFilter::Nearest, VulkanAddress::Repeat, VulkanMipmapMode::Nearest, true);
		//m_SamplerLib.Add("5", VulkanFilter::Nearest, VulkanAddress::ClampToEdge, VulkanMipmapMode::Nearest, true);
	}

	void VulkanDevice::CreateFramebuffer(FramebufferSpecification& frameBufferSpecs, Ref<Framebuffer>& framebuffer)
	{
		auto vkFrameBuffer = std::make_shared<VulkanFramebuffer>(frameBufferSpecs, shared_from_this());
		framebuffer = std::move(vkFrameBuffer);
	}
	
	void VulkanDevice::DestroyFramebuffer(Ref<Framebuffer> framebuffer)
	{
		std::reinterpret_pointer_cast<VulkanFramebuffer>(framebuffer)->CleanUp();
	}

	void VulkanDevice::CreateGraphicsPipeline(GraphicsPipelineSpecification& pipelineSpecs, Ref<GraphicsPipeline>& graphicsPipeline)
	{
		graphicsPipeline = std::make_shared<VulkanGraphicsPipeline>(pipelineSpecs, std::reinterpret_pointer_cast<VulkanDevice>(shared_from_this()));
	}

	void VulkanDevice::CreateBuffer(BufferCreateSpecification& bufferSpecs, Ref<Buffer>& buffer)
	{
		buffer = std::make_shared<VulkanBuffer>(bufferSpecs);
	}

	void VulkanDevice::CreateImage(ImageSpecification& imageSpecs, Ref<Image>& image)
	{
		image = std::make_shared<VulkanImage>(imageSpecs, std::reinterpret_pointer_cast<VulkanDevice>(shared_from_this()));
	}

	void VulkanDevice::AddMipToImage(Ref<Image>& image, MipSpecification& mipSpecs)
	{
		auto vkImage = std::reinterpret_pointer_cast<VulkanImage>(image);
		AllocatedBuffer cpubuffer = {};
		
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.pNext = nullptr;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		
		bufferInfo.size = mipSpecs.MipSize;

		VmaAllocationCreateInfo allocInfo{};

		allocInfo.usage = GetVmaMemoryUsage(MemoryUsage::CPU_To_GPU);

		VulkanAllocator::Allocate(cpubuffer, &bufferInfo, &allocInfo);

		void* mappedMem = nullptr;
		VulkanAllocator::MapMemory(cpubuffer, mappedMem);
		memcpy(mappedMem, mipSpecs.MipData, mipSpecs.MipSize);
		VulkanAllocator::UnMapMemory(cpubuffer);

		ImmediateSubmit([&](VkCommandBuffer buffer)
			{
				VkImageSubresourceRange framebufferRange = {};
				framebufferRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				framebufferRange.baseArrayLayer = 0;
				framebufferRange.baseMipLevel = mipSpecs.mipLevel;
				framebufferRange.layerCount = 1;
				framebufferRange.levelCount = VK_REMAINING_MIP_LEVELS;

				TransitionImageLayout(buffer, vkImage->GetAllocatedImage().Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, framebufferRange);

				VkBufferImageCopy copyRegion{};
				copyRegion.bufferOffset = 0;
				copyRegion.bufferRowLength = mipSpecs.width;
				copyRegion.bufferImageHeight = 0;

				copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				copyRegion.imageSubresource.mipLevel = mipSpecs.mipLevel;
				copyRegion.imageSubresource.baseArrayLayer = 0;
				copyRegion.imageSubresource.layerCount = 1;
				copyRegion.imageExtent.height = mipSpecs.height ;
				copyRegion.imageExtent.width = mipSpecs.width;
				copyRegion.imageExtent.depth = 1;

				vkCmdCopyBufferToImage(buffer, cpubuffer.buffer, vkImage->GetAllocatedImage().Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);


				TransitionImageLayout(buffer, vkImage->GetAllocatedImage().Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, framebufferRange);
			});
		VulkanAllocator::DeAllocate(cpubuffer);
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
	void VulkanDevice::WaitForIdle()
	{
		vkDeviceWaitIdle(m_Device);
	}
	void VulkanDevice::ImmediateSubmit(std::function<void(VkCommandBuffer cmd)>&& func)
	{
		auto vkcmdQueue = std::reinterpret_pointer_cast<VulkanCommandQueue>(m_CommandQueues[QueueType::Graphics][0]);
		auto vkDeviceQueue = std::reinterpret_pointer_cast<VulkanDeviceQueue>(m_DeviceQueues[QueueType::Graphics]);

		VkCommandBuffer cmd;
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandBufferCount = 1;
		allocInfo.commandPool = vkcmdQueue->GetHandle();
		vkAllocateCommandBuffers(m_Device, &allocInfo, &cmd);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optiona

		VkFence fence;
		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = 0;
		vkCreateFence(m_Device, &fenceCreateInfo, nullptr, &fence);

		HY_VK_CHECK(vkBeginCommandBuffer(cmd, &beginInfo));
		func(cmd);
		HY_VK_CHECK(vkEndCommandBuffer(cmd));

		VkSubmitInfo info{};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.pNext = nullptr;
		info.waitSemaphoreCount = 0;
		info.pWaitSemaphores = nullptr;
		info.pWaitDstStageMask = nullptr;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &cmd;
		info.signalSemaphoreCount = 0;
		info.pSignalSemaphores = nullptr;

		HY_VK_CHECK(vkQueueSubmit(vkDeviceQueue->GetHandle(), 1, &info, fence));

		vkWaitForFences(m_Device, 1, &fence, true, UINT64_MAX);
		vkResetFences(m_Device, 1, &fence);

		vkResetCommandBuffer(cmd, 0);
		vkDestroyFence(m_Device, fence, nullptr);
		vkFreeCommandBuffers(m_Device, vkcmdQueue->GetHandle(), 1, &cmd);
	}
}