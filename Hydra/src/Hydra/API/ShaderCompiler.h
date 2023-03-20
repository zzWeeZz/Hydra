#pragma once
#include "Hydra/API/ApiFormats.h"
#include "Hydra/Utils/Memory.h"
#include <string>
#include "dxc/dxcapi.h"
#include <wrl.h>
#include <filesystem>
namespace Hydra
{

	class Shader;
	struct ShaderSpecification;

	struct ShaderCompilerCreateSpecification
	{
		OptimazationMode optimazationMode = OptimazationMode::Debug;
		CompilerBehaviorFlag behaviorFlag = CompilerBehaviorFlag::Default;
	};

	class ShaderCompiler
	{
	public:
		ShaderCompiler(const ShaderCompilerCreateSpecification& creationSpecs)
		{
			m_CreationSpecs = creationSpecs;
			DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(s_Utils.GetAddressOf()));
			DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(s_Compiler.GetAddressOf()));
		}

		virtual ~ShaderCompiler(){}

		virtual CompileErrorFlag Compile(ShaderSpecification& shaderSpecs, Ref<Shader>& outShaderObject, std::string& errorMessage) = 0;

	protected:

		std::wstring GetWstringFromFlag(ShaderStageFlag flag)
		{
			std::wstring wstr;
			switch (flag)
			{
			case Hydra::ShaderStageFlag::Vertex:
				wstr = L"vs_6_5";
				break;
			case Hydra::ShaderStageFlag::Geometry:
				wstr = L"gs_6_5";
				break;
			case Hydra::ShaderStageFlag::Pixel:
				wstr = L"ps_6_5";
				break;
			default:
				HY_CORE_ASSERT(false, "FAILED TO FIND STAGE!");
				break;
			}

			return wstr;
		}

		ShaderCompilerCreateSpecification m_CreationSpecs;
		Microsoft::WRL::ComPtr<IDxcUtils> s_Utils;
		Microsoft::WRL::ComPtr<IDxcCompiler3> s_Compiler;
	};
}