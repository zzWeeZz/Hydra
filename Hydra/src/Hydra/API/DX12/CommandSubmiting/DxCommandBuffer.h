#pragma once
#include "Hydra/API/CommandBuffer.h"
#include "Hydra/API/DX12/DxUtils.h"
namespace Hydra
{
	class DxCommandBuffer : public CommandBuffer
	{
	public:
		DxCommandBuffer(CommandBufferSpecification& specs);

		void Begin() override;
		void End() override;

		[[nodiscard]] FORCEINLINE ID3D12CommandList* GetList() { return reinterpret_cast<ID3D12CommandList*>(m_CommandList.Get()); }
		[[nodiscard]] FORCEINLINE ID3D12CommandList** GetListAddress() { return reinterpret_cast<ID3D12CommandList**>(m_CommandList.GetAddressOf()); }

		[[nodiscard]] FORCEINLINE ID3D12GraphicsCommandList* Get() { return m_CommandList.Get(); }
		[[nodiscard]] FORCEINLINE ID3D12GraphicsCommandList* Get() const { return m_CommandList.Get(); }
		[[nodiscard]] FORCEINLINE ID3D12GraphicsCommandList** GetAddressOf() { return m_CommandList.GetAddressOf(); }

	private:
		WinRef<ID3D12GraphicsCommandList> m_CommandList;
	};
}
