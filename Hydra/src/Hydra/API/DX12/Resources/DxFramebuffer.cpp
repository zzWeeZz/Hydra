#include "HYpch.h"
#include "DxFramebuffer.h"
#include "Hydra/API/DX12/DxUtils.h"
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
		for (size_t i = 0; i < g_FramesInFlight; ++i)
		{
			for (size_t j = 0; j < m_Specs.formats.size(); ++j)
			{

				D3D12_RESOURCE_DESC1 texDesc = {};
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

				D3D12MA::ALLOCATION_DESC allocDesc = {};
				allocDesc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

				DxAllocator::Allocate(m_Images[i][j], texDesc, allocDesc);
			}
		}
	}
}
