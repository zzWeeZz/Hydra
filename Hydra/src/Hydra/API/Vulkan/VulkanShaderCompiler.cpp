#include "HYpch.h"
#include "VulkanShaderCompiler.h"
#include <Hydra/API/Vulkan/Resources/VulkanShader.h>
#include <fstream>
#include <SPIRV-Reflect/spirv_reflect.h>

namespace Hydra
{
	VulkanShaderCompiler::VulkanShaderCompiler(const ShaderCompilerCreateSpecification& specs) : ShaderCompiler(specs)
	{
	}
	CompileErrorFlag VulkanShaderCompiler::Compile(ShaderSpecification& shaderSpecs, Ref<Shader>& outShaderObject, std::string& errorMessage)
	{
		Ref<VulkanShader> vkShader = std::make_shared<VulkanShader>(shaderSpecs);

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


	void VulkanShaderCompiler::CompileStage(const std::filesystem::path& shaderPath, ShaderStageFlag stageFlag, Ref<VulkanShader>& shader)
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

		arguments.push_back(L"-spirv");
		arguments.push_back(L"-fspv-target-env=vulkan1.3");

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

		arguments.push_back(DXC_ARG_PACK_MATRIX_COLUMN_MAJOR);
		if (stageFlag != ShaderStageFlag::Pixel)
		{
			arguments.push_back(L"-fvk-invert-y");
		}

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
		shader->m_SpirvMap[stageFlag].resize(blob->GetBufferSize());
		memcpy(shader->m_SpirvMap[stageFlag].data(), blob->GetBufferPointer(), blob->GetBufferSize());

		Reflect(shader->m_SpirvMap[stageFlag], shader->m_Layouts);
	}


	void VulkanShaderCompiler::Reflect(std::vector<uint32_t> spirv, std::unordered_map<uint32_t, std::vector<VkDescriptorSetLayoutBinding>>& layouts)
	{
		SpvReflectShaderModule spvModule;
		auto reflectResult = spvReflectCreateShaderModule(spirv.size(), reinterpret_cast<void*>(spirv.data()), &spvModule);
		HY_CORE_ASSERT(reflectResult == SPV_REFLECT_RESULT_SUCCESS, "Could not reflect shader.");
		VkShaderStageFlagBits stage = VK_SHADER_STAGE_VERTEX_BIT;

		switch (spvModule.shader_stage)
		{
		case SPV_REFLECT_SHADER_STAGE_COMPUTE_BIT:
		{
			stage = VK_SHADER_STAGE_COMPUTE_BIT;
			break;
		}
		
		case SPV_REFLECT_SHADER_STAGE_VERTEX_BIT:
		{
			stage = VK_SHADER_STAGE_VERTEX_BIT;
			break;
		}

	
		case SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT:
		{
			stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			break;
		}
		}

		uint32_t var_count = 0;
		spvReflectEnumerateInputVariables(&spvModule, &var_count, NULL);
		std::vector<SpvReflectInterfaceVariable*> inputVars(var_count);
		spvReflectEnumerateInputVariables(&spvModule, &var_count, inputVars.data());

		uint32_t spvBindingCount = 0;
		spvReflectEnumerateDescriptorBindings(&spvModule, &spvBindingCount, nullptr);

		for (auto& inputVar : inputVars)
		{
			
		}

		std::vector<SpvReflectDescriptorBinding*> spvBindings(spvBindingCount);
		spvReflectEnumerateDescriptorBindings(&spvModule, &spvBindingCount, spvBindings.data());

		layouts.reserve(spvBindingCount);
		for (uint32_t layoutBindingIndex = 0; layoutBindingIndex < spvBindingCount; ++layoutBindingIndex)
		{
			VkDescriptorType descType{};
			switch (spvBindings[layoutBindingIndex]->descriptor_type)
			{
			case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER:
				descType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				break;
			case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
				descType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				break;

			case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE:
				descType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				break;
			case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				descType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				break;
			default:
				break;
			}
			VkDescriptorSetLayoutBinding descriptorSetLayoutBinding{};
			descriptorSetLayoutBinding.binding = spvBindings[layoutBindingIndex]->binding;
			descriptorSetLayoutBinding.descriptorCount = 1;
			descriptorSetLayoutBinding.descriptorType = descType;
			descriptorSetLayoutBinding.stageFlags = stage;
			descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

			layouts[spvBindings[layoutBindingIndex]->set].push_back(descriptorSetLayoutBinding);
		}
		spvReflectDestroyShaderModule(&spvModule);
	}
}
