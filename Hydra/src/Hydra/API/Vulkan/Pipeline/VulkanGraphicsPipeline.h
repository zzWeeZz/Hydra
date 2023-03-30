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

		HY_GET_INLINE std::unordered_map<uint32_t, std::vector<VkDescriptorSetLayoutBinding>>& GetDescriptorLayout() { return *m_Layouts; }
		[[nodiscard]] VkDescriptorSetLayoutBinding& GetDescriptorSetLayoutBinding(uint32_t space, uint32_t binding);
		HY_GET_INLINE VkPipeline GetHandle() { return m_Pipeline; }
		HY_GET_INLINE VkPipeline GetHandle() const { return m_Pipeline; }
		HY_GET_INLINE VkPipelineLayout GetLayout() { return m_PipelineLayout; }

	private:
		void Validate();
		Ptr<VulkanDevice> m_DeviceHandle;
		VkPipeline m_Pipeline = {};
		VkPipelineLayout m_PipelineLayout = {};
		std::unordered_map<uint32_t, std::vector<VkDescriptorSetLayoutBinding>>* m_Layouts;
	};
}