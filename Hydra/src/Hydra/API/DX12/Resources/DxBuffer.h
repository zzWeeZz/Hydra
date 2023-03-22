#pragma once
#include "Hydra/API/ResourceInterface/Buffer.h"
#include <d3d12.h>
#include "Hydra/API/DX12/DxUtils.h"
#include <Hydra/API/DX12/Backend/DxDevice.h>
#include "Hydra/API/DX12/DxAllocator.h"


namespace Hydra
{
	class DxBuffer : public Buffer
	{
	public:
		DxBuffer(BufferCreateSpecification& specs, Ptr<DxDevice> device);

		// Inherited via Buffer
		void MapMemory(void*& mappedMemory) override;
		void UnmapMemory() override;
		void CopyToBuffer(int32_t frameIndex, void* data, size_t sizeOfData) override;

		HY_GET_INLINE AllocatedBuffer& GetAllocation() { return m_Buffer[0]; }
		HY_GET_INLINE AllocatedBuffer& GetAllocation(uint32_t frameIndex) { return m_Buffer[frameIndex]; }
		HY_GET_INLINE D3D12_GPU_VIRTUAL_ADDRESS& GetGPUAddress() { return m_BufferLocation; }
		HY_GET_INLINE UINT& GetSizeInBytes() { return m_SizeInBytes; }
		HY_GET_INLINE UINT& StrideInBytes() { return m_StrideInBytes; }
		HY_GET_INLINE DXGI_FORMAT& IndexFormat() { return m_Format; }

	private:
		D3D12_RESOURCE_STATES GetStateFromUsage(BufferUsage usage);
		void Create();
		void CreateTypeVertexBufferIndexBuffer();
		void CreateConstantBuffer();
		PerFrameInFlight<AllocatedBuffer> m_Buffer;

		D3D12_GPU_VIRTUAL_ADDRESS m_BufferLocation;
		UINT m_SizeInBytes;
		UINT m_StrideInBytes;
		DXGI_FORMAT m_Format;

		Ptr<DxDevice> m_Device;
	};
}