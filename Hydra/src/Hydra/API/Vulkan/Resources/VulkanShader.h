#pragma once
#include <vulkan/vulkan.h>
#include <Hydra/API/ApiFormats.h>
#include "Hydra/API/ResourceInterface/Shader.h"
namespace Hydra
{
	class VulkanShader : public Shader
	{
		friend class VulkanShaderCompiler;
		friend class VulkanGraphicsPipeline;
	public:
		VulkanShader(ShaderSpecification& specs);

		[[nodiscard]] std::vector<uint32_t> const& GetSpirvFromVkShaderStage(VkShaderStageFlagBits flag);

	private:
		std::unordered_map<ShaderStageFlag, std::vector<uint32_t>> m_SpirvMap;
		std::unordered_map<uint32_t, std::vector<VkDescriptorSetLayoutBinding>> m_Layouts;
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, uint32_t>> m_BindingLookup; // use this to look up a binding in a given set.
		std::vector<VkVertexInputAttributeDescription> m_VertexInputAttributes;
		std::vector<VkShaderStageFlagBits> m_VkStages;
	};
}
