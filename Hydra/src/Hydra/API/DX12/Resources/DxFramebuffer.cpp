#include "HYpch.h"
#include "DxFramebuffer.h"
#include "Hydra/API/DX12/DxUtils.h"
#include <Hydra/API/DX12/Backend/DxDevice.h>
#include <d3dx12.h>
namespace Hydra
{
	DxFramebuffer::DxFramebuffer(FramebufferSpecification& specs, Ptr<Device> device) : Framebuffer(specs, device)
	{
		for (size_t i = 0; i < g_FramesInFlight; i++)
		{
			m_RenderTargets[i].resize(m_Specs.formats.size());
		}
		Validate();
	}
	void DxFramebuffer::Validate()
	{
		auto dxDevice = std::reinterpret_pointer_cast<DxDevice>(m_Device.lock());

		rect.bottom = m_Specs.height;
		rect.right = m_Specs.width;
		rect.left = 0;
		rect.top = 0;

		viewPort = {};

		viewPort.Height = m_Specs.height;
		viewPort.Width = m_Specs.width;
		viewPort.MinDepth = 0;
		viewPort.MaxDepth = 1;
		
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = g_FramesInFlight;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		HY_DX_CHECK(dxDevice->Get()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(m_RtvDescriptorHeap.GetAddressOf())));

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		m_RtvDescriptorSize = dxDevice->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		for (size_t i = 0; i < g_FramesInFlight; ++i)
		{
			for (size_t j = 0; j < m_Specs.formats.size(); ++j)
			{

				D3D12_RESOURCE_DESC texDesc = {};
				texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
				texDesc.Width = m_Specs.width;
				texDesc.Height = m_Specs.height;
				texDesc.DepthOrArraySize = 1;
				texDesc.MipLevels = 1;
				texDesc.Format = GetDxFormat(m_Specs.formats[j]);
				texDesc.SampleDesc.Count = 1;
				texDesc.SampleDesc.Quality = 0;
				texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
				texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

				D3D12_HEAP_PROPERTIES HeapProps = {};
				HeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
				HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
				HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
				HeapProps.CreationNodeMask = 1;
				HeapProps.VisibleNodeMask = 1;

				FLOAT clearColor[4] = { 0.32f, 0.32f, 0.32f, 1.f };
				D3D12_CLEAR_VALUE clrValue{};
				clrValue.Format = GetDxFormat(m_Specs.formats[j]);
				clrValue.Color[0] = clearColor[0];
				clrValue.Color[1] = clearColor[1];
				clrValue.Color[2] = clearColor[2];
				clrValue.Color[3] = clearColor[3];

				HY_DX_CHECK(dxDevice->Get()->CreateCommittedResource(
					&HeapProps,
					D3D12_HEAP_FLAG_NONE,
					&texDesc,
					D3D12_RESOURCE_STATE_RENDER_TARGET,
					&clrValue,
					IID_PPV_ARGS(m_RenderTargets[i][j].ReleaseAndGetAddressOf())
				));

				dxDevice->Get()->CreateRenderTargetView(m_RenderTargets[i][j].Get(), nullptr, rtvHandle);

				rtvHandle.Offset(1, static_cast<UINT>(m_RtvDescriptorSize));
			}
		}
	}
}
