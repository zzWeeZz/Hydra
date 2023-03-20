#pragma once
#include <filesystem>

#include "Hydra/API/Resource.h"
#include "Hydra/API/ApiFormats.h"
namespace Hydra
{
	struct ShaderStage
	{
		std::filesystem::path filepath;
		std::vector<std::string> CompileDefines;
	};

	struct ShaderSpecification
	{
		ShaderStage vertexShaderStage;
		ShaderStage geometryShaderStage;
		ShaderStage pixelShaderStage;
	};

	class Shader : public Resource
	{
	public:
		Shader(ShaderSpecification& specs)
		{
			m_Specs = specs;
			if (specs.vertexShaderStage.filepath.empty() == false)
			{
				m_Stages.emplace_back(ShaderStageFlag::Vertex);
			}
			if (specs.geometryShaderStage.filepath.empty() == false)
			{
				m_Stages.emplace_back(ShaderStageFlag::Geometry);
			}
			if (specs.pixelShaderStage.filepath.empty() == false)
			{
				m_Stages.emplace_back(ShaderStageFlag::Pixel);
			}
		}
		virtual ~Shader(){}

		[[nodiscard]] FORCEINLINE std::vector<ShaderStageFlag> const& GetStage() { return m_Stages; }

	protected:
		ShaderSpecification m_Specs;
		std::vector<ShaderStageFlag> m_Stages;
	};
}