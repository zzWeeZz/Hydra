#pragma once
#include "Hydra/API/ShaderCompiler.h"
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

	};
}