#pragma once
#include "Hydra/API/Framebuffer.h"
#include <Hydra/API/DX12/DxAllocator.h>
namespace Hydra
{
	class DxFramebuffer : public Framebuffer
	{
	public:
		DxFramebuffer(FramebufferSpecification& specs, Ptr<Device> device);
	private:
		void Validate();
		PerFrameInFlight<std::vector<AllocatedImage>> m_Images;
	};
}
