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
		virtual void MapMemory(void*& mappedMemory) override;
		virtual void UnmapMemory() override;
	private:

		void Create();
		AllocatedBuffer m_Buffer;

		D3D12_GPU_VIRTUAL_ADDRESS m_BufferLocation;
		UINT m_SizeInBytes;
		UINT m_StrideInBytes;
		DXGI_FORMAT m_Format;

		Ptr<DxDevice> m_Device;
	};
}