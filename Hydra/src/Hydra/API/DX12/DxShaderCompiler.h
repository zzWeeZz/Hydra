#pragma once
#include "Hydra/API/ShaderCompiler.h"
#include <d3d12.h>
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

		void ReflectInputLayout(Microsoft::WRL::ComPtr<IDxcResult> compileResult, std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout);
		void ReflectRootParameters(Microsoft::WRL::ComPtr<IDxcResult> compileResult, std::vector<D3D12_ROOT_PARAMETER>& outRootPrameters);
	};
}
