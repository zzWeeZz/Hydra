#pragma once


#include "vma/vk_mem_alloc.h"
#include <deque>
#include <functional>
#include <unordered_map>
#include <mutex>

namespace Hydra
{
	struct AllocatedBuffer
	{
		uint32_t id;
		VkBuffer buffer;
		VmaAllocation allocation;
		VkDeviceSize sizeOfBuffer;
	};

	struct AllocatedImage
	{
		uint32_t id;
		VkImage Image;
		VmaAllocation allocation;
		VkDeviceSize sizeOfBuffer;
	};

	class VulkanAllocator
	{
	public:
		static void Initialize(VkPhysicalDevice physicalDevice, VkDevice device, VkInstance instance);
		static void Allocate(AllocatedBuffer& allocation, VkBufferCreateInfo* bufferInfo, VmaAllocationCreateInfo* allocationInfo);
		static void Allocate(AllocatedImage& allocation, VkImageCreateInfo* imageInfo, VmaAllocationCreateInfo* allocationInfo);
		static void DeAllocate(AllocatedBuffer& allocation);
		static void DeAllocate(AllocatedImage& allocation);
		static void MapMemory(AllocatedBuffer& allocation, void*& mappedMemory);
		static void UnMapMemory(AllocatedBuffer& allocation);
		static void CustomDeletion(std::function<void()>&& func);
		static void Flush();
		static void Shutdown();


	private:
		inline static VmaAllocator s_Allocator;
		inline static uint32_t s_ID;
		inline static std::unordered_map<uint32_t, std::function<void()>> s_DestroyFunctions;
		inline static std::vector<uint32_t> s_AllocateDestructorOrder;
		inline static std::deque<std::function<void()>> s_DestructionQueue;
		inline static std::mutex s_AllocationMutex;
	};
}