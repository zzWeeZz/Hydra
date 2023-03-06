#include "HYpch.h"
#include "GraphicsContext.h"
#include <Hydra/API/DX12/Backend/DxPhysicalDevice.h>
#include <Hydra/API/DX12/Backend/DxDevice.h>
#include <Hydra/API/DX12/Backend/DxSwapchain.h>
namespace Hydra
{
	void GraphicsContext::Initalize(const GraphicsContextSpecification& specs)
	{
		if (specs.api == API::Dx12)
		{
			PhysicalDeviceSpecifications pdSpecs{};
			s_PhysicalDevice = (std::make_shared<DxPhysicalDevice>(pdSpecs));
			s_Device = (std::make_shared<DxDevice>(s_PhysicalDevice));
			SwapchainSpecfications specs{};
			specs.frameCount = 3;
			specs.device = s_Device;
			specs.physicalDevice = s_PhysicalDevice;
			s_Swapchain = (std::make_shared<DxSwapchain>(specs));
		}
	}
}