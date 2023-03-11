#pragma once
namespace Hydra
{
	constexpr size_t InvalidResourceID = ~0;

	class Resource
	{
	public:
		[[nodiscard]] FORCEINLINE bool IsValid() const { return m_ResourceID != InvalidResourceID; }
		[[nodiscard]] FORCEINLINE size_t GetResourceID() const { return m_ResourceID; }
	protected:
		size_t m_ResourceID = ~0;
	};
}