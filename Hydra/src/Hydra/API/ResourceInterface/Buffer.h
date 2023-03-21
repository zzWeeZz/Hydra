#pragma once
#include "Hydra/API/Resource.h"
#include "Hydra/API/ApiFormats.h"
#include <cstdint>
namespace Hydra
{
	struct BufferCreateSpecification
	{
		void* data = nullptr;
		size_t size = 0;
		uint32_t stride = 0;
		BufferUsage usage = BufferUsage::None;
		MemoryUsage allocationUsage;
	};

	class Buffer : public Resource
	{
	public:
		Buffer(BufferCreateSpecification& specs) { m_Specs = specs; }
		virtual ~Buffer(){}
		virtual void MapMemory(void*& mappedMemory) = 0;
		virtual void UnmapMemory() = 0;

	protected:
		BufferCreateSpecification m_Specs;
	};
}