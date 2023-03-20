#include "HYpch.h"
#include "VulkanShader.h"
namespace Hydra
{
	VulkanShader::VulkanShader(ShaderSpecification& specs) : Shader(specs)
	{
		for (auto& stage : m_Stages)
		{
			m_SpirvMap[stage] = {};
			VkShaderStageFlagBits& shaderBit = m_VkStages.emplace_back();
			switch (stage)
			{
			case ShaderStageFlag::Vertex:
				shaderBit = VK_SHADER_STAGE_VERTEX_BIT;
				break;
			case ShaderStageFlag::Geometry:
				shaderBit = VK_SHADER_STAGE_GEOMETRY_BIT;
				break;
			case ShaderStageFlag::Pixel:
				shaderBit = VK_SHADER_STAGE_FRAGMENT_BIT;
				break;
			default:
				break;
			}
		}
	}
	std::vector<uint32_t> const& VulkanShader::GetSpirvFromVkShaderStage(VkShaderStageFlagBits flag)
	{
		std::vector<uint32_t>& returnVec = m_SpirvMap[ShaderStageFlag::Vertex];

		switch (flag)
		{
		case VK_SHADER_STAGE_VERTEX_BIT:
			returnVec = m_SpirvMap[ShaderStageFlag::Vertex];
			break;

		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
			break;

		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
			break;

		case VK_SHADER_STAGE_GEOMETRY_BIT:
			returnVec = m_SpirvMap[ShaderStageFlag::Geometry];
			break;

		case VK_SHADER_STAGE_FRAGMENT_BIT:
			returnVec = m_SpirvMap[ShaderStageFlag::Pixel];
			break;

		case VK_SHADER_STAGE_COMPUTE_BIT:
			break;

		default:
			break;
		}
		return returnVec;
	}
}