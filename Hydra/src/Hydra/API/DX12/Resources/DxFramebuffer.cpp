#include "HYpch.h"
#include "DxFramebuffer.h"
#include "Hydra/API/DX12/DxUtils.h"
#include <Hydra/API/DX12/Backend/DxDevice.h>
namespace Hydra
{
	DxFramebuffer::DxFramebuffer(FramebufferSpecification& specs, Ptr<Device> device) : Framebuffer(specs, device)
	{
		for (size_t i = 0; i < g_FramesInFlight; i++)
		{
			m_Images[i].resize(m_Specs.formats.size());
		}
		Validate();
	}
	void DxFramebuffer::Validate()
	{
		auto dxDevice = std::reinterpret_pointer_cast<DxDevice>(m_Device.lock());

		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = g_FramesInFlight;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		HY_DX_CHECK(dxDevice->Get()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(m_RtvDescriptorHeap.GetAddressOf())));

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

				FLOAT clearColor[4] = { 0.32f, 0.32f, 0.32f, 1.f };
				D3D12_CLEAR_VALUE clrValue{};
				clrValue.Format = GetDxFormat(m_Specs.formats[j]);
				clrValue.Color[0] = clearColor[0];
				clrValue.Color[1] = clearColor[1];
				clrValue.Color[2] = clearColor[2];
				clrValue.Color[3] = clearColor[3];

				D3D12MA::ALLOCATION_DESC allocDesc = {};
				allocDesc.HeapType = D3D12_HEAP_TYPE_DEFAULT;
				allocDesc.ExtraHeapFlags = D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES;

				DxAllocator::Allocate(m_Images[i][j], texDesc, allocDesc, clrValue);
			}
		}
	}
}
