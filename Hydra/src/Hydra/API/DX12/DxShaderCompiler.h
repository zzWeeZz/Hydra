#pragma once
#include "Hydra/API/ShaderCompiler.h"
namespace Hydra
{
	class DxShader;

	class DxShaderCompiler : public ShaderCompiler
	{
	public:
		DxShaderCompiler(ShaderCompilerCreateSpecification& createSpecs);

		// Inherited via ShaderCompiler
		CompileErrorFlag Compile(ShaderSpecification& shaderSpecs, Ref<Shader>& outShaderObject, std::string& errorMessage) override;
	private:
		void CompileStage(const std::filesystem::path& shaderPath, ShaderStageFlag stageFlag, Ref<DxShader>& shader);
	};
}
