#pragma once
#include "Hydra/API/ShaderCompiler.h"
#include <unordered_map>
#include <vector>
#include "vulkan/vulkan.h"
#include <SPIRV-Reflect/spirv_reflect.h>

namespace Hydra
{
	class VulkanShader;
	class VulkanShaderCompiler : public ShaderCompiler
	{
	public:
		VulkanShaderCompiler(const ShaderCompilerCreateSpecification& specs);
		// Inherited via ShaderCompiler
		CompileErrorFlag Compile(ShaderSpecification& shaderSpecs, Ref<Shader>& outShaderObject, std::string& errorMessage) override;
	private:

		void CompileStage(const std::filesystem::path& shaderPath, ShaderStageFlag stageFlag, Ref<VulkanShader>& shader);
		void Reflect(std::vector<uint32_t> spirv, std::unordered_map<uint32_t, std::vector<VkDescriptorSetLayoutBinding>>& layouts, Ref<VulkanShader>& shader);

		void ReflectVertexAttributes(SpvReflectShaderModule& spvModule, Ref<Hydra::VulkanShader>& shader);

		VkFormat GetVkFormatFromReflectFormat(SpvReflectFormat& reflect);
	};
}