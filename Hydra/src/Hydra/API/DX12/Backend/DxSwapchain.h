#pragma once
#include "Hydra/API/Swapchain.h"
#include "Hydra/API/DX12/DxUtils.h"

namespace Hydra
{
	class DxSwapchain : public Swapchain
	{
	public:
		DxSwapchain(const SwapchainSpecfications& specs);
		
		uint32_t PrepareNewFrame() override;

		// Inherited via Swapchain
		void Resize(uint32_t width, uint32_t height) override;

		void Create(Ptr<Context> context) override;

		void Validate(Ptr<Context> context) override;
		void Present() override;


		[[nodiscard]] FORCEINLINE ID3D12Fence1* GetFence() { return m_Fences[m_CurrentFrame].Get(); }
		[[nodiscard]] FORCEINLINE ID3D12Fence1** GetFenceAddress() { return m_Fences[m_CurrentFrame].GetAddressOf(); }

		[[nodiscard]] FORCEINLINE size_t& GetFenceValue() { return m_FenceValues[m_CurrentFrame]; }

	private:
		WinRef<IDXGISwapChain3> m_Swapchain;
		WinRef<ID3D12DescriptorHeap> m_RTVDescriptorHeap;
		PerFrameInFlight<WinRef<ID3D12Resource>> m_RenderTargets;
		PerFrameInFlight<size_t> m_FenceValues;
		PerFrameInFlight<WinRef<ID3D12Fence1>> m_Fences;

		PerFrameInFlight<HANDLE> m_FenceEvent;

		// Inherited via Swapchain
	};
}