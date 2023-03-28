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
		DxAllocator::MapMemory(m_Buffer[0], mappedMemory);
	}

	void DxBuffer::UnmapMemory()
	{
		DxAllocator::UnMapMemory(m_Buffer[0]);
	}
	void DxBuffer::CopyToBuffer(int32_t frameIndex, void* data, size_t sizeOfData, size_t offsetIndex)
	{
		auto& currentAlloc = m_Buffer[frameIndex];

		CD3DX12_RANGE readRange(0, 0);    // We do not intend to read from this resource on the CPU. (End is less than or equal to begin)
		size_t cBufferOffset = (sizeOfData + 255) & ~255;

		uint8_t* mappedMemory = nullptr;
		DxAllocator::MapMemory(m_Buffer[frameIndex], reinterpret_cast<void*&>(mappedMemory));
		memcpy_s(mappedMemory + offsetIndex * cBufferOffset, sizeOfData, data, sizeOfData);
		DxAllocator::UnMapMemory(m_Buffer[frameIndex]);
	}
	D3D12_RESOURCE_STATES DxBuffer::GetStateFromUsage(BufferUsage usage)
	{
		D3D12_RESOURCE_STATES state = {};

		switch (usage)
		{
		case Hydra::BufferUsage::None:
			HY_CORE_ASSERT(false, "Buffer usage CANT be 'None'. Set the buffer usages to something valid!");
			break;
		case Hydra::BufferUsage::VertexBuffer:
			state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
			break;
		case Hydra::BufferUsage::IndexBuffer:
			state = D3D12_RESOURCE_STATE_INDEX_BUFFER;
			break;
		case Hydra::BufferUsage::ConstantBuffer:
			state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
			break;
		case Hydra::BufferUsage::StorageBuffer:
			break;
		case Hydra::BufferUsage::IndirectBuffer:
			state = D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
			break;
		default:
			break;
		}

		return state;
	}
	void DxBuffer::Create()
	{
		if (m_Specs.usage == BufferUsage::IndexBuffer || m_Specs.usage == BufferUsage::VertexBuffer)
		{
			CreateTypeVertexBufferIndexBuffer();
		}
		else
		{
			CreateConstantBuffer();
		}
	}
	void DxBuffer::CreateTypeVertexBufferIndexBuffer()
	{
		const size_t sizeOfBuffer = m_Specs.count * m_Specs.stride;
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
		  .HeapType = D3D12_HEAP_TYPE_UPLOAD
		};


		DxAllocator::Allocate(m_Buffer[0], desc, allocation_desc, D3D12_RESOURCE_STATE_GENERIC_READ);

		m_BufferLocation = m_Buffer[0].buffer->GetGPUVirtualAddress();
		m_SizeInBytes = static_cast<UINT>(sizeOfBuffer);

		m_StrideInBytes = static_cast<UINT>(m_Specs.stride);
		if (m_Specs.usage == BufferUsage::IndexBuffer)
		{
			m_Format = DXGI_FORMAT_R16_UINT;
		}

		if (m_Specs.data)
		{
			void* mappedMemory = nullptr;
			DxAllocator::MapMemory(m_Buffer[0], mappedMemory);
			memcpy(mappedMemory, m_Specs.data, sizeOfBuffer);
			DxAllocator::UnMapMemory(m_Buffer[0]);
		}
	}
	void DxBuffer::CreateConstantBuffer()
	{
		const size_t sizeOfBuffer = m_Specs.count * m_Specs.stride;
		m_StrideInBytes = static_cast<UINT>(m_Specs.stride);

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
		  .HeapType = D3D12_HEAP_TYPE_UPLOAD
		};


		for (auto& alloc : m_Buffer)
		{
			DxAllocator::Allocate(alloc, desc, allocation_desc, D3D12_RESOURCE_STATE_GENERIC_READ);
		}
	}
}