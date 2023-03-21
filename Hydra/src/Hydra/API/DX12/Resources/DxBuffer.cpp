#include "HYpch.h"
#include "DxBuffer.h"
#include <d3dx12.h>
namespace Hydra
{
	DxBuffer::DxBuffer(BufferCreateSpecification& specs, Ptr<DxDevice> device) : Buffer(specs)
	{
		m_Device = device;
		Create();
	}

	void DxBuffer::MapMemory(void*& mappedMemory)
	{
	}

	void DxBuffer::UnmapMemory()
	{
	}
	void DxBuffer::Create()
	{
		const size_t sizeOfBuffer = m_Specs.size * m_Specs.stride;

		D3D12_RESOURCE_DESC desc = {
			.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
			.Width = sizeOfBuffer,
			.Height = 1,
			.DepthOrArraySize = 1,
			.MipLevels = 1,
			.Format = DXGI_FORMAT_UNKNOWN,
			.SampleDesc = {
				.Count = 1
			},
			.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR
		};

		D3D12MA::ALLOCATION_DESC allocation_desc = {
		  .HeapType = D3D12_HEAP_TYPE_DEFAULT
		};

		DxAllocator::Allocate(m_Buffer, desc, allocation_desc, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		m_BufferLocation = m_Buffer.buffer->GetGPUVirtualAddress();
		m_SizeInBytes = static_cast<UINT>(sizeOfBuffer);

		m_StrideInBytes = static_cast<UINT>(m_Specs.stride);
	}
}