#pragma once
#include "Hydra/API/Device.h"
#include "Hydra/API/PhysicalDevice.h"
#include "Hydra/API/Swapchain.h"
namespace Hydra
{
	struct ContextSpecification
	{
		size_t width = 1280;
		size_t height = 720;
		bool debuging = true;
		std::vector<QueueType> queueTypes;
	};

	class Context : public std::enable_shared_from_this<Context>
	{
	public:
		virtual void Initalize(const ContextSpecification& specs) = 0;

		Ptr<Device> GetDevice() { return m_Device; }
		Ptr<PhysicalDevice> GetPhyicalDevice() { return m_PhysicalDevice; }
		Ptr<Swapchain> GetSwapchain() { return m_Swapchain; }

	protected:
		Ref<Device> m_Device;
		Ref<PhysicalDevice> m_PhysicalDevice;
		Ref<Swapchain> m_Swapchain;
	 	ContextSpecification m_Specs;
	};
}