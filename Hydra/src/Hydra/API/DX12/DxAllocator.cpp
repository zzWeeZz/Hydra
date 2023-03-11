#include "HYpch.h"
#include "DxAllocator.h"
#include "Hydra/API/DX12/DxUtils.h"
namespace Hydra
{
	void DxAllocator::Initialize(IDXGIAdapter1* adapter, ID3D12Device2* device)
	{
		D3D12MA::ALLOCATOR_DESC allocDesc = {};
		allocDesc.pAdapter = adapter;
		allocDesc.pDevice = device;
		allocDesc.Flags = D3D12MA::ALLOCATOR_FLAG_NONE;

		HY_DX_CHECK(D3D12MA::CreateAllocator(&allocDesc, &s_Allcator));
		
	}
}