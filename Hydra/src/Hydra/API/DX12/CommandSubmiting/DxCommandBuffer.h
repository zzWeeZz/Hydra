#pragma once
#include "Hydra/API/CommandBuffer.h"
#include "Hydra/API/DX12/DxUtils.h"
namespace Hydra
{
	class DxCommandBuffer : public CommandBuffer
	{
	public:
		DxCommandBuffer(CommandBufferSpecification& specs);

		[[nodiscard]] FORCEINLINE ID3D12CommandList* Get() { return m_CommandList.Get(); }
		[[nodiscard]] FORCEINLINE ID3D12CommandList* Get() const { return m_CommandList.Get(); }
		[[nodiscard]] FORCEINLINE ID3D12CommandList** GetAddressOf() { return m_CommandList.GetAddressOf(); }

	private:
		WinRef<ID3D12CommandList> m_CommandList;
	};
}
