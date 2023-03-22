#pragma once
#include "Hydra/API/ResourceInterface/Shader.h"
#include <dxc/dxcapi.h>
#include "d3d12.h"
#include <vector>
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
		std::vector<D3D12_INPUT_ELEMENT_DESC> m_VertexShaderInputElements;
		std::vector<D3D12_ROOT_PARAMETER> m_ReflectedRootPrameters;
	};
}
