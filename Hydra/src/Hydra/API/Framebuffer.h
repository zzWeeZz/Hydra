#pragma once
#include "Hydra/API/Resource.h"
#include "Hydra/API/ApiFormats.h"
#include <vector>
#include <Hydra/API/Device.h>

namespace Hydra
{
	struct FramebufferSpecification
	{
		uint32_t width;
		uint32_t height;
		std::vector<ImageFormat> formats;
	};

	class Framebuffer : public Resource
	{
		friend class CommandBuffer;
	public:
		Framebuffer(FramebufferSpecification& specs, Ptr<Device> device)
		{
			m_Specs = specs;
			m_Device = device;
		}
		
	protected:
		FramebufferSpecification m_Specs;
		Ptr<Device> m_Device;
	};
}