#pragma once
#include <vulkan/vulkan.h>

#include "Hydra/API/PipelineInterface/GraphicsPipeline.h"
#include "Hydra/API/Vulkan/VulkanUtils.h"


namespace Hydra
{
	class VulkanDevice;

	class VulkanGraphicsPipeline : public GraphicsPipeline
	{
	public:
		VulkanGraphicsPipeline(const GraphicsPipelineSpecification& specs, Ptr<VulkanDevice> device);


		HY_GET_INLINE VkPipeline GetHandle() { return m_Pipeline; }
		HY_GET_INLINE VkPipeline GetHandle() const { return m_Pipeline; }
		HY_GET_INLINE VkPipelineLayout GetLayout() { return m_PipelineLayout; }

	private:
		void Validate();
		Ptr<VulkanDevice> m_DeviceHandle;
		VkPipeline m_Pipeline = {};
		VkPipelineLayout m_PipelineLayout = {};
	};
}