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


		[[nodiscard]] FORCEINLINE VkPipeline GetHandle() { return m_Pipeline; }
		[[nodiscard]] FORCEINLINE VkPipeline GetHandle() const { return m_Pipeline; }

	private:
		void Validate();
		Ptr<VulkanDevice> m_DeviceHandle;
		VkPipeline m_Pipeline = {};
		VkPipelineLayout m_PipelineLayout = {};
	};
}