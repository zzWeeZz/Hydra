#include "HYpch.h"
#include "DxAllocator.h"
#include "Hydra/API/DX12/DxUtils.h"
namespace Hydra
{
#if 1
#define HY_ALLOC_PRINT(...) HY_CORE_INFO(__VA_ARGS__)
#else
#define HY_ALLOC_PRINT(...)
#endif

	void DxAllocator::Initialize(IDXGIAdapter1* adapter, ID3D12Device2* device)
	{
		D3D12MA::ALLOCATOR_DESC allocDesc = {};
		allocDesc.pAdapter = adapter;
		allocDesc.pDevice = device;
		allocDesc.Flags = D3D12MA::ALLOCATOR_FLAG_NONE;

		HY_DX_CHECK(D3D12MA::CreateAllocator(&allocDesc, &s_Allcator));
		s_ID = 14124;
	}

	void DxAllocator::Allocate(AllocatedImage& image, D3D12_RESOURCE_DESC& resourceDesc, D3D12MA::ALLOCATION_DESC& allocationDesc, D3D12_CLEAR_VALUE& clearValue)
	{
		HY_DX_CHECK(
			s_Allcator->CreateResource(
				&allocationDesc,
				&resourceDesc,
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				nullptr,
				&image.allocation,
				IID_PPV_ARGS(&image.texture)));
		image.id = s_ID++;
		image.sizeOfBuffer = image.allocation->GetSize();
		HY_ALLOC_PRINT("DxAllocator: id {0} Allocating image: {1} bytes", image.id, image.sizeOfBuffer);
		s_AllocateDestructorOrder.push_back(image.id);
		s_DestroyFunctions[image.id] = [&, image]()
		{
			image.texture->Release();
			image.allocation->Release();
			HY_ALLOC_PRINT("DxAllocator: id {0} Deallocating image: {1} bytes", image.id, image.sizeOfBuffer);
		};
	}

	void DxAllocator::DeAllocate(AllocatedImage& image)
	{
		image.texture->Release();
		image.allocation->Release();
		s_DestroyFunctions.erase(image.id);
		HY_ALLOC_PRINT("DxAllocator: id {0} Deallocating image: {1} bytes", image.id, image.sizeOfBuffer);
		auto it = std::find(s_AllocateDestructorOrder.begin(), s_AllocateDestructorOrder.end(), image.id);
		if (it != s_AllocateDestructorOrder.end())
		{
			s_AllocateDestructorOrder.erase(it);
		}
	}

	void DxAllocator::Flush()
	{
		for (int32_t Index = static_cast<int32_t>(s_AllocateDestructorOrder.size() - 1u); Index >= 0; Index--)
		{
			s_DestroyFunctions[s_AllocateDestructorOrder[Index]]();
		}

		for (auto& it : s_DestructionQueue)
		{
			it();
		}
		s_DestructionQueue.clear();
	}

	void DxAllocator::Shutdown()
	{
		s_Allcator->Release();
	}
}