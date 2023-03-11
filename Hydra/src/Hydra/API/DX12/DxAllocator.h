#pragma once
#include "dma/D3D12MemAlloc.h"
#include <dxgi1_6.h>
namespace Hydra
{
	class DxAllocator
	{
	public:
		static void Initialize(IDXGIAdapter1* adapter, ID3D12Device2* device);
	private:
		static inline D3D12MA::Allocator* s_Allcator;
	};
}
