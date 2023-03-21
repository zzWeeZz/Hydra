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

		[[nodiscard]] FORCEINLINE AllocatedBuffer& GetAllocation() { return m_Buffer; }

	private:

		void Allocate();

		AllocatedBuffer m_Buffer;
	};
}