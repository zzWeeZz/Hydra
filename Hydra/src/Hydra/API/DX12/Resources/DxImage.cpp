#include "HYpch.h"
#include "DxImage.h"
#include <d3dx12.h>
#include "Hydra/API/DX12/Backend/DxDevice.h"
#include "Hydra/API/DX12/DxUtils.h"
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
		texDesc.MipLevels = 5;
		texDesc.Format = GetDxFormat( m_Specs.format);
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		texDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		D3D12MA::ALLOCATION_DESC desc = {};
		desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

		DxAllocator::Allocate(m_Image, texDesc, desc, D3D12_RESOURCE_STATE_COPY_DEST);

		UINT64 textureUploadBufferSize;
		dxDevice->Get()->GetCopyableFootprints(
			&texDesc,
			0, // FirstSubresource
			1, // NumSubresources
			0, // BaseOffset
			nullptr, // pLayouts
			nullptr, // pNumRows
			nullptr, // pRowSizeInBytes
			&textureUploadBufferSize); // pTotalBytes

		
		desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
		D3D12_RESOURCE_DESC textureUploadResourceDesc = {};
		textureUploadResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		textureUploadResourceDesc.Alignment = 0;
		textureUploadResourceDesc.Width = textureUploadBufferSize;
		textureUploadResourceDesc.Height = 1;
		textureUploadResourceDesc.DepthOrArraySize = 1;
		textureUploadResourceDesc.MipLevels = 1;
		textureUploadResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		textureUploadResourceDesc.SampleDesc.Count = 1;
		textureUploadResourceDesc.SampleDesc.Quality = 0;
		textureUploadResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		textureUploadResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		AllocatedImage uploadImage = {};

		DxAllocator::Allocate(uploadImage, textureUploadResourceDesc, desc, D3D12_RESOURCE_STATE_GENERIC_READ);
		
		dxDevice->ImmediateSubmit([&](ID3D12GraphicsCommandList* cmd)
			{

				D3D12_SUBRESOURCE_DATA textureSubresourceData = {};
				textureSubresourceData.pData = m_Specs.ImageData;
				textureSubresourceData.RowPitch = m_Specs.width * 4;
				textureSubresourceData.SlicePitch = m_Specs.width * m_Specs.height * 4;

				UpdateSubresources(cmd, m_Image.texture, uploadImage.texture, 0, 0, 1, &textureSubresourceData);

				D3D12_RESOURCE_BARRIER textureBarrier = {};
				textureBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				textureBarrier.Transition.pResource = m_Image.texture;
				textureBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
				textureBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
				textureBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				cmd->ResourceBarrier(1, &textureBarrier);
			});

		
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
