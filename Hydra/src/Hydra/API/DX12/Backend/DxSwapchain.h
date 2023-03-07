#pragma once
#include "Hydra/API/Swapchain.h"
#include "Hydra/API/DX12/DxUtils.h"

namespace Hydra
{
	class DxSwapchain : public Swapchain
	{
	public:
		DxSwapchain(const SwapchainSpecfications& specs);

		// Inherited via Swapchain
		void Resize(uint32_t width, uint32_t height) override;

		void Create(Ptr<Context> context) override;

		void Validate(Ptr<Context> context) override;

	private:
		WinRef<IDXGISwapChain3> m_Swapchain;
	};
}