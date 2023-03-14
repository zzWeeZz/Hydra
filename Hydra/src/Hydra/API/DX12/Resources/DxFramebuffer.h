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
	private:
		void Validate();
		WinRef<ID3D12DescriptorHeap> m_RtvDescriptorHeap;
		PerFrameInFlight<std::vector<AllocatedImage>> m_Images;
	};
}
