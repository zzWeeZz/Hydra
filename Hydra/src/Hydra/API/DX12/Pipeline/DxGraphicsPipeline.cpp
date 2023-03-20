#include "HYpch.h"
#include "DxGraphicsPipeline.h"
#include <d3d12.h>
#include <d3dx12.h>
#include <Hydra/API/DX12/Resources/DxShader.h>
#include <Hydra/API/DX12/Backend/DxDevice.h>
namespace Hydra
{
	DxGraphicsPipeline::DxGraphicsPipeline(const GraphicsPipelineSpecification& specs, Ptr<DxDevice> dxDevice) : GraphicsPipeline(specs)
	{
		m_DeviceHandle = dxDevice;
		Validate();
	}
	void DxGraphicsPipeline::Validate()
	{
		auto dxShader = std::reinterpret_pointer_cast<DxShader>(m_Specs.shaderObject.lock());
		std::vector<D3D12_ROOT_PARAMETER> params;

		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.Init(params.size(),
			params.data(),
			0,
			nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		ID3DBlob* signature = nullptr;
		ID3DBlob* errorBlob = nullptr;
		HY_DX_CHECK(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &signature, &errorBlob));
		HY_DX_CHECK(m_DeviceHandle.lock()->Get()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(m_RootSignature.GetAddressOf())));
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayoutDesc;
		//vsCompiler.GetInputLayout(inputLayoutDesc);
		D3D12_INPUT_LAYOUT_DESC inputLayout;
		inputLayout.NumElements = static_cast<UINT>(inputLayoutDesc.size());
		inputLayout.pInputElementDescs = inputLayoutDesc.data();

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
		pipelineDesc.VS = {dxShader->m_BlobMap[ShaderStageFlag::Vertex]->GetBufferPointer()	,dxShader->m_BlobMap[ShaderStageFlag::Vertex]->GetBufferSize() };
		pipelineDesc.PS = { dxShader->m_BlobMap[ShaderStageFlag::Pixel]->GetBufferPointer(),dxShader->m_BlobMap[ShaderStageFlag::Pixel]->GetBufferSize() };
		pipelineDesc.pRootSignature = m_RootSignature.Get();
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineDesc.InputLayout = inputLayout;
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pipelineDesc.NumRenderTargets = 1;
		pipelineDesc.SampleMask = 0xffffffff;
		pipelineDesc.SampleDesc.Count = 1;
		//pipelineDesc.DSVFormat = (DXGI_FORMAT)45;
		//pipelineDesc.DepthStencilState = GetDepthStencilDesc(info.depthState, info.depthCullState);

		pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineDesc.RasterizerState.FrontCounterClockwise = FALSE;
		pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		HY_DX_CHECK(m_DeviceHandle.lock()->Get()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(m_PipelineStateObject.GetAddressOf())));
	}
}