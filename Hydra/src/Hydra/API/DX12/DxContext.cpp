#include "HYpch.h"
#include "DxContext.h"
#include <Hydra/API/DX12/Backend/DxPhysicalDevice.h>
#include <Hydra/API/DX12/Backend/DxDevice.h>
#include <Hydra/API/DX12/Backend/DxSwapchain.h>
namespace Hydra
{
	void DxContext::Initalize(const ContextSpecification& specs)
	{
		PhysicalDeviceSpecifications pdSpecs = {};
		m_PhysicalDevice = std::make_shared<DxPhysicalDevice>(pdSpecs);
		m_Device = std::make_shared<DxDevice>(m_PhysicalDevice);
		SwapchainSpecfications scSpecs = {};
		scSpecs.context = shared_from_this();
		m_Swapchain = std::make_shared<DxSwapchain>(scSpecs);
	}
}
