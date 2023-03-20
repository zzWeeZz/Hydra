#pragma once
#include "Hydra/API/ResourceInterface/Shader.h"
#include <dxc/dxcapi.h>
namespace Hydra
{
	class DxShader : public Shader
	{
		friend class DxShaderCompiler;
		friend class DxGraphicsPipeline;
	public:
		DxShader(ShaderSpecification& specs);


	private:
		std::unordered_map<ShaderStageFlag, IDxcBlob*> m_BlobMap;
	};
}
