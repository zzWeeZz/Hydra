#include "HYpch.h"
#include "DxGraphicsPipeline.h"
#include <d3d12.h>
#include <d3dx12.h>
#include <Hydra/API/DX12/Resources/DxShader.h>
#include <Hydra/API/DX12/Backend/DxDevice.h>
#include <Hydra/API/DX12/Resources/DxFramebuffer.h>
namespace Hydra
{
	DxGraphicsPipeline::DxGraphicsPipeline(const GraphicsPipelineSpecification& specs, Ptr<DxDevice> dxDevice) : GraphicsPipeline(specs)
	{
		m_DeviceHandle = dxDevice;
		Validate();
	}

	// from the book Introduction to 3d Game programming with DirectX 12 by Frank D. Luna

	std::array<const CD3DX12_STATIC_SAMPLER_DESC,
		2> GetStaticSamplers()
	{
		// Applications usually only need a handful of samplers.So just define them
			// all up front and keep them available as part of the root signature.
		const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
			2, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressU
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressV
			D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW
		const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
			3, // shaderRegister
			D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressU
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressV
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW
		//const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		//	2, // shaderRegister
		//	D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		//	D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressU
		//	D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressV
		//	D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW
		//const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		//	3, // shaderRegister
		//	D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		//	D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressU
		//	D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressV
		//	D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW
		//const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		//	4, // shaderRegister
		//	D3D12_FILTER_ANISOTROPIC, // filter
		//	D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressU
		//	D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressV
		//	D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressW
		//	0.0f, // mipLODBias
		//	8); // maxAnisotropy
		//const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		//	5, // shaderRegister
		//	D3D12_FILTER_ANISOTROPIC, // filter
		//	D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressU
		//	D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressV
		//	D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressW
		//	0.0f, // mipLODBias
		//	8); // maxAnisotropy
		return {
		pointWrap, pointClamp
		/*anisotropicWrap, anisotropicClamp*/ };
	}

	void DxGraphicsPipeline::Validate()
	{
		auto dxShader = std::reinterpret_pointer_cast<DxShader>(m_Specs.shaderObject.lock());
		auto dxFramebuffer = std::reinterpret_pointer_cast<DxFramebuffer>(m_Specs.framebufferObject.lock());
		std::vector<D3D12_ROOT_PARAMETER> params = dxShader->m_ReflectedRootPrameters;
		auto samplers = GetStaticSamplers();
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.Init(static_cast<uint32_t>(params.size()),
			params.data(),
			samplers.size(),
			samplers.data(),
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);



		ID3DBlob* signature = nullptr;
		ID3DBlob* errorBlob = nullptr;
		HY_DX_CHECK(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &signature, &errorBlob));
		HY_DX_CHECK(m_DeviceHandle.lock()->Get()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(m_RootSignature.GetAddressOf())));

		std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayoutDesc = dxShader->m_VertexShaderInputElements;


		D3D12_INPUT_LAYOUT_DESC inputLayout = {};
		inputLayout.NumElements = static_cast<UINT>(inputLayoutDesc.size());
		inputLayout.pInputElementDescs = inputLayoutDesc.data();

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

		for (size_t i = 0; auto& format : dxFramebuffer->m_Specs.formats)
		{
			pipelineDesc.RTVFormats[i] = GetDxFormat(format);
			i++;
		}
		pipelineDesc.NumRenderTargets = dxFramebuffer->m_Specs.formats.size();


		pipelineDesc.VS = { dxShader->m_BlobMap[ShaderStageFlag::Vertex]->GetBufferPointer() ,dxShader->m_BlobMap[ShaderStageFlag::Vertex]->GetBufferSize() };
		pipelineDesc.PS = { dxShader->m_BlobMap[ShaderStageFlag::Pixel]->GetBufferPointer(),dxShader->m_BlobMap[ShaderStageFlag::Pixel]->GetBufferSize() };

		pipelineDesc.pRootSignature = m_RootSignature.Get();

		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		pipelineDesc.InputLayout = inputLayout;

		pipelineDesc.SampleMask = 0xffffffff;
		pipelineDesc.SampleDesc.Count = 1;

		if (dxFramebuffer->m_HasDepth)
		{
			pipelineDesc.DSVFormat = dxFramebuffer->m_DepthFormat;
			pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		}

		pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineDesc.RasterizerState.FrontCounterClockwise = TRUE;
		pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		pipelineDesc.BlendState.AlphaToCoverageEnable = TRUE;
		HY_DX_CHECK(m_DeviceHandle.lock()->Get()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(m_PipelineStateObject.GetAddressOf())));
	}
}