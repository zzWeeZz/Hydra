#pragma once
#include "dma/D3D12MemAlloc.h"
#include <dxgi1_6.h>
namespace Hydra
{
	struct AllocatedBuffer
	{
		uint32_t id;
		ID3D12Resource2* buffer;
		D3D12MA::Allocation allocation;
		size_t sizeOfBuffer;
	};

	struct AllocatedImage
	{
		uint32_t id;
		ID3D12Resource2* texture;
		D3D12MA::Allocation allocation;
		size_t sizeOfBuffer;
	};

	class DxAllocator
	{
	public:
		static void Initialize(IDXGIAdapter1* adapter, ID3D12Device2* device);
	private:
		static inline D3D12MA::Allocator* s_Allcator;
	};
}
