#include "HYpch.h"
#include "DxShaderCompiler.h"
#include <iostream>
#include <fstream>
#include "Hydra/API/DX12/Resources/DxShader.h"
namespace Hydra
{
	DxShaderCompiler::DxShaderCompiler(ShaderCompilerCreateSpecification& createSpecs) : ShaderCompiler(createSpecs)
	{
	}
	CompileErrorFlag DxShaderCompiler::Compile(ShaderSpecification& shaderSpecs, Ref<Shader>& outShaderObject, std::string& errorMessage)
	{
		Ref<DxShader> vkShader = std::make_shared<DxShader>(shaderSpecs);

		if (shaderSpecs.vertexShaderStage.filepath.empty() == false)
		{
			CompileStage(shaderSpecs.vertexShaderStage.filepath, ShaderStageFlag::Vertex, vkShader);
		}

		if (shaderSpecs.geometryShaderStage.filepath.empty() == false)
		{
			CompileStage(shaderSpecs.geometryShaderStage.filepath, ShaderStageFlag::Geometry, vkShader);
		}
		if (shaderSpecs.pixelShaderStage.filepath.empty() == false)
		{
			CompileStage(shaderSpecs.pixelShaderStage.filepath, ShaderStageFlag::Pixel, vkShader);
		}
		outShaderObject = vkShader;
		return CompileErrorFlag::Success;
	}
	void DxShaderCompiler::CompileStage(const std::filesystem::path& shaderPath, ShaderStageFlag stageFlag, Ref<DxShader>& shader)
	{
		std::vector<LPCWSTR> arguments;
		arguments.emplace_back(L"-E");
		arguments.emplace_back(L"main");


		Microsoft::WRL::ComPtr<IDxcBlobEncoding> sourceBlob;
		// Get the whole file to string.
		std::ifstream fin(shaderPath, std::ios::binary | std::ios::ate);
		fin.seekg(0, std::ios::end);
		std::string file;
		file.resize(fin.tellg());
		fin.seekg(0, std::ios::beg);
		fin.read(file.data(), file.size());

		s_Utils->CreateBlob(file.data(), static_cast<uint32_t>(file.size()), CP_UTF8, sourceBlob.GetAddressOf());
		auto target = GetWstringFromFlag(stageFlag);
		arguments.push_back(L"-T");
		arguments.push_back(target.c_str());


		switch (m_CreationSpecs.optimazationMode)
		{
		case OptimazationMode::Debug:
			arguments.push_back(DXC_ARG_SKIP_OPTIMIZATIONS);
			break;
		case OptimazationMode::Performance:
			arguments.push_back(DXC_ARG_SKIP_VALIDATION);
			break;
		case OptimazationMode::Size:
			break;
		default:
			break;
		}

		switch (m_CreationSpecs.behaviorFlag)
		{
		case CompilerBehaviorFlag::Default:
			break;
		case CompilerBehaviorFlag::TreatWarningsAsErrors:
			arguments.push_back(DXC_ARG_WARNINGS_ARE_ERRORS);
			break;
		default:
			break;
		}

		arguments.push_back(DXC_ARG_PACK_MATRIX_ROW_MAJOR);
		/*if (stageFlag != ShaderStageFlag::Pixel)
		{
			arguments.push_back(L"-fvk-invert-y");
		}*/

		DxcBuffer sourceBuffer{};
		sourceBuffer.Ptr = sourceBlob->GetBufferPointer();
		sourceBuffer.Size = sourceBlob->GetBufferSize();
		sourceBuffer.Encoding = 0;

		Microsoft::WRL::ComPtr<IDxcResult> compileResult;
		auto hr = s_Compiler->Compile(&sourceBuffer, arguments.data(), (uint32_t)arguments.size(), nullptr, IID_PPV_ARGS(compileResult.GetAddressOf()));
		if (FAILED(hr))
		{
			HY_CORE_ASSERT(false, "HLSL File did not compile!");
		}


		Microsoft::WRL::ComPtr<IDxcBlobUtf8> pErrors;
		IDxcBlobWide* wide = nullptr;
		hr = compileResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(pErrors.GetAddressOf()), &wide);
		if (pErrors && pErrors->GetStringLength() > 0)
		{
			HY_CORE_ERROR("HLSL crashed, {}", (char*)pErrors->GetBufferPointer());
		}
		IDxcBlob* blob = nullptr;
		compileResult->GetResult(&blob);

		shader->m_BlobMap[stageFlag] = blob;
	}
}