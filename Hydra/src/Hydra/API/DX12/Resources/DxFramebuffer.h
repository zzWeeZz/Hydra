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

		[[nodiscard]] FORCEINLINE ID3D12DescriptorHeap* GetHeap() { return m_RtvDescriptorHeap.Get(); }
		[[nodiscard]] FORCEINLINE size_t DescriptorSize() { return m_RtvDescriptorSize; }
		[[nodiscard]] FORCEINLINE ID3D12Resource* GetResource(uint32_t frameindex) { return m_RenderTargets[frameindex][0].Get(); }
	private:
		void Validate();
		WinRef<ID3D12DescriptorHeap> m_RtvDescriptorHeap;
		PerFrameInFlight<std::vector<WinRef<ID3D12Resource>>> m_RenderTargets;

		size_t m_RtvDescriptorSize = 0;
	};
}
