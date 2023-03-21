#pragma once
#include "Hydra/API/Framebuffer.h"
#include <Hydra/API/DX12/DxAllocator.h>
#include "Hydra/API/DX12/DxUtils.h"
namespace Hydra
{
	class DxFramebuffer : public Framebuffer
	{
	public:
		DxFramebuffer(FramebufferSpecification& specs, Ptr<Device> device);

		void Resize(const size_t width, const size_t height) override;

		[[nodiscard]] FORCEINLINE D3D12_VIEWPORT& GetViewport() { return viewPort; }
		[[nodiscard]] FORCEINLINE D3D12_RECT& GetRect() { return rect; }

		[[nodiscard]] FORCEINLINE ID3D12DescriptorHeap* GetHeap() { return m_RtvDescriptorHeap.Get(); }
		[[nodiscard]] FORCEINLINE UINT DescriptorSize() { return static_cast<UINT>(m_RtvDescriptorSize); }
		[[nodiscard]] FORCEINLINE ID3D12Resource* GetResource(uint32_t frameindex) { return m_RenderTargets[frameindex][0].Get(); }
	private:
		void Validate();
		WinRef<ID3D12DescriptorHeap> m_RtvDescriptorHeap;
		PerFrameInFlight<std::vector<WinRef<ID3D12Resource>>> m_RenderTargets;
		D3D12_VIEWPORT viewPort = {};
		D3D12_RECT rect = {};
		size_t m_RtvDescriptorSize = 0;
	};
}
