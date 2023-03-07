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
		void Initialize(VkPhysicalDevice physicalDevice, VkDevice device, VkInstance instance);
		void Allocate(AllocatedBuffer& allocation, VkBufferCreateInfo* bufferInfo, VmaAllocationCreateInfo* allocationInfo);
		void Allocate(AllocatedImage& allocation, VkImageCreateInfo* imageInfo, VmaAllocationCreateInfo* allocationInfo);
		void DeAllocate(AllocatedBuffer& allocation);
		void DeAllocate(AllocatedImage& allocation);

		void MapMemory(AllocatedBuffer& allocation, void*& mappedMemory);
		void UnMapMemory(AllocatedBuffer& allocation);
		void QueueDeletion(std::function<void()>&& func);
		void Flush();
		void Shutdown();


	private:
		VmaAllocator s_Allocator;
		uint32_t s_ID;
		std::unordered_map<uint32_t, std::function<void()>> s_DestroyFunctions;
		std::vector<uint32_t> s_AllocateDestructorOrder;
		std::deque<std::function<void()>> s_DestructionQueue;
		std::mutex s_AllocationMutex;
	};
}