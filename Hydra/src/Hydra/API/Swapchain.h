#pragma once
#include <cstdint>
#include "Hydra/API/PhysicalDevice.h"
#include "Hydra/API/Device.h"
#include "Hydra/Utils/Memory.h"
namespace Hydra
{
	class Context;
	struct SwapchainSpecfications
	{
		size_t width = 0;
		size_t height = 0;
		size_t frameCount = 3;
		Ptr<Context> context;
	};
	class Context;
	class Swapchain
	{
	public:
		Swapchain(const SwapchainSpecfications& specs) { m_Specs = specs; }
		virtual ~Swapchain(){}

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Create(Ptr<Context> context) = 0;
		virtual void Present() = 0;
		virtual void Validate(Ptr<Context> context) = 0;
		
	protected:
		
		uint32_t m_CurrentImage{};

		SwapchainSpecfications m_Specs;
	};
}