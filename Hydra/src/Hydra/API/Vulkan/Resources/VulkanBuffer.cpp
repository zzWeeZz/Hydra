#include "HYpch.h"
#include "VulkanBuffer.h"
#include "Hydra/API/Vulkan/VulkanUtils.h"
namespace Hydra
{
	VulkanBuffer::VulkanBuffer(BufferCreateSpecification& specs) : Buffer(specs)
	{
		Allocate();
	}
	void VulkanBuffer::MapMemory(void*& mappedMemory)
	{
		HY_CORE_ASSERT(m_Buffer.buffer != nullptr, "The buffer needs to be allocated before you try to map it!");
		VulkanAllocator::MapMemory(m_Buffer, mappedMemory);
	}
	void VulkanBuffer::UnmapMemory()
	{
		HY_CORE_ASSERT(m_Buffer.buffer != nullptr, "The buffer needs to be allocated before you try to unmap it!");
		VulkanAllocator::UnMapMemory(m_Buffer);
	}
	void VulkanBuffer::Allocate()
	{
		const size_t bufferSize = m_Specs.size * m_Specs.stride;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.pNext = nullptr;
		bufferInfo.usage = GetVkBufferUsage(m_Specs.usage);
		bufferInfo.size = bufferSize;

		VmaAllocationCreateInfo allocInfo{};

		allocInfo.usage = GetVmaMemoryUsage(m_Specs.allocationUsage);

		VulkanAllocator::Allocate(m_Buffer, &bufferInfo, &allocInfo);

		if (m_Specs.data)
		{
			void* mappedMemory = nullptr;
			VulkanAllocator::MapMemory(m_Buffer, mappedMemory);
			memcpy_s(mappedMemory, bufferSize, m_Specs.data, bufferSize);
			VulkanAllocator::UnMapMemory(m_Buffer);
		}
	}
}