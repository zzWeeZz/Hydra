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
		// This function will setup all backend systems to a new frame state. OBS: You will need to handle the frame index! if the frame index is -1 that mean that something didnt work when 
		// making the new frame. you CANT run any other API functions if this function returns -1.
		virtual int32_t PrepareNewFrame() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Create(Ptr<Context> context) = 0;
		virtual void Present() = 0;
		virtual void Validate(Ptr<Context> context) = 0;
		
	protected:
		
		int32_t m_CurrentImage{};
		uint32_t m_CurrentFrame{};
		SwapchainSpecfications m_Specs;
	};
}