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

	private:
		WinRef<IDXGISwapChain3> m_Swapchain;
		WinRef<ID3D12DescriptorHeap> m_RTVDescriptorHeap;
		PerFrameInFlight<WinRef<ID3D12Resource2>> m_RenderTargets;
		PerFrameInFlight<size_t> m_FenceValues;
		PerFrameInFlight<WinRef<ID3D12Fence1>> m_Fences;

		HANDLE m_FenceEvent;

		// Inherited via Swapchain
	};
}