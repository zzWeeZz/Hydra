#include "HYpch.h"
#include "DxImage.h"
#include <d3dx12.h>
#include "Hydra/API/DX12/Backend/DxDevice.h"
namespace Hydra
{
	DxImage::DxImage(ImageSpecification& specs, Ptr<DxDevice> device)
	{
		m_Specs = specs;
		m_Device = device;
		Validate();
	}

	void DxImage::Validate()
	{
		auto dxDevice = std::reinterpret_pointer_cast<DxDevice>(m_Device.lock());

		D3D12_RESOURCE_DESC texDesc = {};
		texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		texDesc.Width = m_Specs.width;
		texDesc.Height = m_Specs.height;
		texDesc.DepthOrArraySize = 1;
		texDesc.MipLevels = 1;
		texDesc.Format = GetDxFormat(m_Specs.format);
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

		D3D12MA::ALLOCATION_DESC desc = {};
		desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

		DxAllocator::Allocate(m_Image, texDesc, desc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = 1;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		HY_DX_CHECK(dxDevice->Get()->CreateDescriptorHeap(&rtvHeapDesc, HY_DX_ID(m_DescriptorHeap)));

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = GetDxFormat(m_Specs.format);
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		D3D12_CPU_DESCRIPTOR_HANDLE descHandle = {
			m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart() };
		dxDevice->Get()->CreateShaderResourceView(m_Image.texture, &srvDesc, descHandle);
	}


}
