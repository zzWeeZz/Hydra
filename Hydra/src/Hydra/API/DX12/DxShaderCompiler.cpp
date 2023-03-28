#include "HYpch.h"
#include "DxShaderCompiler.h"
#include <iostream>
#include <fstream>
#include "Hydra/API/DX12/Resources/DxShader.h"
#include <d3dcompiler.h>
#include <Hydra/API/DX12/DxUtils.h>
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

		arguments.push_back(DXC_ARG_PACK_MATRIX_COLUMN_MAJOR);
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

		if (stageFlag == ShaderStageFlag::Vertex)
		{
			ReflectInputLayout(compileResult, shader->m_VertexShaderInputElements);
		}
		ReflectRootParameters(compileResult, shader->m_ReflectedRootPrameters);
	}
	void DxShaderCompiler::ReflectInputLayout(Microsoft::WRL::ComPtr<IDxcResult> compileResult, std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout)
	{
		ID3D12ShaderReflection* pReflector = nullptr;
		WinRef<IDxcBlob> pReflectionData;
		compileResult->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(pReflectionData.GetAddressOf()), 0);
		DxcBuffer reflectionBuffer{};
		reflectionBuffer.Ptr = pReflectionData->GetBufferPointer();
		reflectionBuffer.Size = pReflectionData->GetBufferSize();
		reflectionBuffer.Encoding = 0;
		s_Utils->CreateReflection(&reflectionBuffer, IID_PPV_ARGS(&pReflector));
		D3D12_SHADER_DESC decs;
		pReflector->GetDesc(&decs);

		std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayoutDesc;
		for (UINT i = 0; i < decs.InputParameters; i++)
		{
			D3D12_SIGNATURE_PARAMETER_DESC paramDesc;
			pReflector->GetInputParameterDesc(i, &paramDesc);
			std::string f3C(paramDesc.SemanticName);
			f3C = f3C.substr(0, 3);
			if (f3C == "SV_")
			{
				continue;
			}
			// Fill out input element desc
			D3D12_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;
			// determine DXGI format
			if (paramDesc.Mask == 1)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (paramDesc.Mask <= 3)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (paramDesc.Mask <= 7)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (paramDesc.Mask <= 15)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			// Save element desc
			inputLayout.push_back(elementDesc);
		}
	}
	void DxShaderCompiler::ReflectRootParameters(Microsoft::WRL::ComPtr<IDxcResult> compileResult, std::vector<D3D12_ROOT_PARAMETER>& outRootPrameters)
	{
		ID3D12ShaderReflection* pReflector = nullptr;
		WinRef<IDxcBlob> pReflectionData;
		compileResult->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(pReflectionData.GetAddressOf()), 0);
		DxcBuffer reflectionBuffer{};
		reflectionBuffer.Ptr = pReflectionData->GetBufferPointer();
		reflectionBuffer.Size = pReflectionData->GetBufferSize();
		reflectionBuffer.Encoding = 0;
		s_Utils->CreateReflection(&reflectionBuffer, IID_PPV_ARGS(&pReflector));

		D3D12_SHADER_DESC desc;
		pReflector->GetDesc(&desc);


		for (size_t i = 0; i < desc.BoundResources; ++i)
		{
			D3D12_SHADER_INPUT_BIND_DESC inputBinDesc;
			pReflector->GetResourceBindingDesc(i, &inputBinDesc);
			if (inputBinDesc.Type == D3D10_SIT_CBUFFER)
			{
				auto& range = outRootPrameters.emplace_back();
				range.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
				range.Descriptor = { inputBinDesc.BindPoint, inputBinDesc.Space };
				range.ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX;
			}

			else if (inputBinDesc.Type == D3D10_SIT_TEXTURE )
			{
				auto& range = outRootPrameters.emplace_back();
				range.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				range.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
				D3D12_DESCRIPTOR_RANGE* tableRange = new D3D12_DESCRIPTOR_RANGE();
				tableRange->RangeType = inputBinDesc.Type == D3D10_SIT_TEXTURE ? D3D12_DESCRIPTOR_RANGE_TYPE_SRV : D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
				tableRange->NumDescriptors = inputBinDesc.BindCount;
				tableRange->BaseShaderRegister = inputBinDesc.BindPoint;
				tableRange->RegisterSpace = inputBinDesc.Space;
				tableRange->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
				range.DescriptorTable.pDescriptorRanges = tableRange;
				range.DescriptorTable.NumDescriptorRanges = 1;
			}
		}
	}
}