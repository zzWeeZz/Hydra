#pragma once
#include "dma/D3D12MemAlloc.h"
#include <dxgi1_6.h>
#include <deque>
namespace Hydra
{
	struct AllocatedBuffer
	{
		uint32_t id;
		ID3D12Resource* buffer;
		D3D12MA::Allocation* allocation;
		size_t sizeOfBuffer;
	};

	struct AllocatedImage
	{
		uint32_t id;
		ID3D12Resource* texture;
		D3D12MA::Allocation* allocation;
		size_t sizeOfBuffer;
	};

	class DxAllocator
	{
	public:
		static void Initialize(IDXGIAdapter1* adapter, ID3D12Device2* device);
		static void Allocate(AllocatedImage& image, D3D12_RESOURCE_DESC& resourceDesc, D3D12MA::ALLOCATION_DESC& allocationDesc, D3D12_CLEAR_VALUE* clearValue);
		static void Allocate(AllocatedBuffer& image, D3D12_RESOURCE_DESC& resourceDesc, D3D12MA::ALLOCATION_DESC& allocationDesc, D3D12_RESOURCE_STATES intialState);
		static void DeAllocate(AllocatedImage& image);
		static void DeAllocate(AllocatedBuffer& buffer);

		static void MapMemory(AllocatedBuffer& buffer, void*& mappedMemory);
		static void UnMapMemory(AllocatedBuffer& buffer);

		static void Flush();
		static void Shutdown();
	private:
		static inline D3D12MA::Allocator* s_Allcator;
		static inline D3D12MA::Pool* s_Pool;
		inline static uint32_t s_ID;
		inline static std::unordered_map<uint32_t, std::function<void()>> s_DestroyFunctions;
		inline static std::vector<uint32_t> s_AllocateDestructorOrder;
		inline static std::deque<std::function<void()>> s_DestructionQueue;
	};
}
