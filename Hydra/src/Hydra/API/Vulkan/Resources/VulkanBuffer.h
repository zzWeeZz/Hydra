#pragma once
#include "Hydra/API/ResourceInterface/Buffer.h"
#include <Hydra/API/Vulkan/VulkanAllocator.h>

namespace Hydra
{
	class VulkanBuffer : public Buffer
	{
	public:
		VulkanBuffer(BufferCreateSpecification& specs);
		// Inherited via Buffer
		virtual void MapMemory(void*& mappedMemory) override;
		virtual void UnmapMemory() override;

		void CopyToBuffer(int32_t frameIndex, void* data, size_t sizeOfData, size_t offsetIndex = 0) override;

		[[nodiscard]] FORCEINLINE AllocatedBuffer& GetAllocation() { return m_Buffer; }

	private:

		void Allocate();

		AllocatedBuffer m_Buffer;
	};
}