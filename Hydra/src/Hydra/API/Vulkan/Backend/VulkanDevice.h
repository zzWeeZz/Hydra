#pragma once
#include "Hydra/API/Device.h"
#include "Vulkan/vulkan.h"
#include "Hydra/API/PhysicalDevice.h"
#include <Hydra/API/Vulkan/Backend/VulkanPhysicalDevice.h>
#include <vector>
#include <Hydra/API/Vulkan/VulkanAllocator.h>
#include "Hydra/API/Vulkan/VulkanUtils.h"
#include <Hydra/API/Vulkan/VulkanDeviceQueue.h>
namespace Hydra
{
	class VulkanDevice : public Device
	{
	public:
		VulkanDevice(Ptr<PhysicalDevice> physicalDevice);

		void Create(Ref<VulkanPhysicalDevice> physicalDevice, const std::vector<const char*> validationLayer);
		VkDevice GetHandle() { return m_Device; }

		void CreateFramebuffer(FramebufferSpecification& frameBufferSpecs, Ref<Framebuffer>& framebuffer) override;
		void DestroyFramebuffer(Ref<Framebuffer> framebuffer) override;

		void CreateGraphicsPipeline(GraphicsPipelineSpecification& pipelineSpecs, Ref<GraphicsPipeline>& graphicsPipeline) override;

		void CreateBuffer(BufferCreateSpecification& bufferSpecs, Ref<Buffer>& buffer) override;

		void CreateImage(ImageSpecification& imageSpecs, Ref<Image>& image) override;

		void Shutdown();

		void WaitForIdle();

		void ImmediateSubmit(std::function<void(VkCommandBuffer cmd)>&& func);

	private:
		void CreateCommandPools(Ref<VulkanPhysicalDevice> physicalDevice, size_t amount = 1);
		bool CheckDeviceExtensionSupport(Ref<VulkanPhysicalDevice> physicalDevice, const std::vector<const char*> deviceExtensions);

		VkDevice m_Device;

		// Inherited via Device
	};
}