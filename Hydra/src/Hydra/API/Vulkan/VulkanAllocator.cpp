#include "HYpch.h"
#define VMA_IMPLEMENTATION
#include "VulkanAllocator.h"
#include "Hydra/API/Vulkan/VulkanContext.h"
#include <Hydra/API/Vulkan/VulkanUtils.h>


#if TN_ALLOCATOR_TRACE
#define TN_ALLOC_PRINT(...) TN_CORE_INFO(__VA_ARGS__)
#else
#define TN_ALLOC_PRINT(...)
#endif

namespace Hydra
{
	void VulkanAllocator::Initialize(VkPhysicalDevice physicalDevice, VkDevice device, VkInstance instance)
	{
		VmaAllocatorCreateInfo createInfo{};
		createInfo.device = device;
		createInfo.instance = instance;
		createInfo.physicalDevice = physicalDevice;
		s_ID = 0;
		HY_VK_CHECK(vmaCreateAllocator(&createInfo, &s_Allocator));
	}

	void VulkanAllocator::Allocate(AllocatedBuffer& allocation, VkBufferCreateInfo* bufferInfo, VmaAllocationCreateInfo* allocationInfo)
	{
		std::scoped_lock lock(s_AllocationMutex);

		allocation.id = s_ID;
		TN_ALLOC_PRINT("TitanAllocator: id {0} Allocating buffer: {1} bytes", allocation.id, bufferInfo->size);
		allocation.sizeOfBuffer = bufferInfo->size;
		HY_VK_CHECK(vmaCreateBuffer(s_Allocator, bufferInfo, allocationInfo, &allocation.buffer, &allocation.allocation, nullptr));
		s_AllocateDestructorOrder.push_back(allocation.id);
		s_DestroyFunctions[allocation.id] = [&, allocation]() {TN_ALLOC_PRINT("TitanAllocator: id {0} Deallocating buffer: {1} bytes", allocation.id, allocation.sizeOfBuffer); vmaDestroyBuffer(s_Allocator, allocation.buffer, allocation.allocation); };

		s_ID++;
	}

	void VulkanAllocator::Allocate(AllocatedImage& allocation, VkImageCreateInfo* imageInfo, VmaAllocationCreateInfo* allocationInfo)
	{
		std::scoped_lock lock(s_AllocationMutex);

		allocation.id = s_ID;
		HY_VK_CHECK(vmaCreateImage(s_Allocator, imageInfo, allocationInfo, &allocation.Image, &allocation.allocation, nullptr));
		VmaAllocationInfo allocInfo{};
		vmaGetAllocationInfo(s_Allocator, allocation.allocation, &allocInfo);
		allocation.sizeOfBuffer = allocInfo.size;
		// uses a vector to keep track of the order of the allocations.
		s_AllocateDestructorOrder.push_back(allocation.id);
		TN_ALLOC_PRINT("TitanAllocator: id {0} Allocating image: {1} bytes", allocation.id, allocation.sizeOfBuffer);
		// inorder to track and manage the allocations that VMA does. and this gives a deallocation function to the map.
		s_DestroyFunctions[allocation.id] = [&, allocation]() {TN_ALLOC_PRINT("TitanAllocator: id {0} Deallocating image: {1} bytes", allocation.id, allocation.sizeOfBuffer); vmaDestroyImage(s_Allocator, allocation.Image, allocation.allocation); };


		s_ID++;
	}

	void VulkanAllocator::DeAllocate(AllocatedBuffer& allocation)
	{
		std::scoped_lock lock(s_AllocationMutex);

		TN_ALLOC_PRINT("TitanAllocator: id {0} Deallocating buffer: {1} bytes", allocation.id, allocation.sizeOfBuffer);
		vmaDestroyBuffer(s_Allocator, allocation.buffer, allocation.allocation);
		s_DestroyFunctions.erase(allocation.id);
		auto it = std::find(s_AllocateDestructorOrder.begin(), s_AllocateDestructorOrder.end(), allocation.id);
		if (it != s_AllocateDestructorOrder.end())
		{
			s_AllocateDestructorOrder.erase(it);
		}
	}

	void VulkanAllocator::DeAllocate(AllocatedImage& allocation)
	{
		std::scoped_lock lock(s_AllocationMutex);

		TN_ALLOC_PRINT("TitanAllocator: id {0} Deallocating image: {1} bytes", allocation.id, allocation.sizeOfBuffer);
		vmaDestroyImage(s_Allocator, allocation.Image, allocation.allocation);
		s_DestroyFunctions.erase(allocation.id);
		auto it = std::find(s_AllocateDestructorOrder.begin(), s_AllocateDestructorOrder.end(), allocation.id);
		if (it != s_AllocateDestructorOrder.end())
		{
			s_AllocateDestructorOrder.erase(it);
		}
	}

	void VulkanAllocator::MapMemory(AllocatedBuffer& allocation, void*& mappedMemory)
	{
		HY_VK_CHECK(vmaMapMemory(s_Allocator, allocation.allocation, &mappedMemory));
	}

	void VulkanAllocator::UnMapMemory(AllocatedBuffer& allocation)
	{
		vmaUnmapMemory(s_Allocator, allocation.allocation);
	}

	void VulkanAllocator::CustomDeletion(std::function<void()>&& func)
	{
		s_DestructionQueue.push_back(func);
	}

	void VulkanAllocator::Flush()
	{
		std::scoped_lock lock(s_AllocationMutex);

		for (int32_t Index = static_cast<int32_t>(s_AllocateDestructorOrder.size() - 1u); Index >= 0; Index--)
		{	
			s_DestroyFunctions[s_AllocateDestructorOrder[Index]]();
		}

		for (auto & it : s_DestructionQueue)
		{
			it();
		}
		s_DestructionQueue.clear();
	}
	void VulkanAllocator::Shutdown()
	{
		vmaDestroyAllocator(s_Allocator);
	}
}