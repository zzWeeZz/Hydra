#pragma once
#include "Hydra/API/Device.h"
#include "Hydra/API/PhysicalDevice.h"
#include "Hydra/API/Swapchain.h"
namespace Hydra
{
	struct GraphicsContextSpecification
	{
		size_t width = 1280;
		size_t height = 720;
		bool debuging = true;
	};

	class Context
	{
	public:
		virtual void Initalize(const GraphicsContextSpecification& specs) = 0;

		Ptr<Device> GetDevice() { return m_Device; }
		Ptr<PhysicalDevice> GetPhyicalDevice() { return m_PhysicalDevice; }
		Ptr<Swapchain> GetSwapchain() { return m_Swapchain; }

	protected:
		Ref<Device> m_Device;
		Ref<PhysicalDevice> m_PhysicalDevice;
		Ref<Swapchain> m_Swapchain;
	 	GraphicsContextSpecification m_Specs;
	};
}