#pragma once
#include "Hydra/API/Resource.h"
#include "Hydra/API/ApiFormats.h"
#include <cstdint>
#include "Hydra/API/ApiUtils.h"
namespace Hydra
{
	struct BufferCreateSpecification
	{
		void* data = nullptr;
		size_t count = 0;
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
		virtual void CopyToBuffer(int32_t frameIndex, void* data, size_t sizeOfData, size_t offsetIndex = 0) = 0;

		HY_GET_INLINE uint32_t GetStride() { return m_Specs.stride; }

		[[nodiscard]] FORCEINLINE uint32_t GetBufferSize() { return static_cast<uint32_t>(m_Specs.count); }

	protected:
		BufferCreateSpecification m_Specs;
	};
}