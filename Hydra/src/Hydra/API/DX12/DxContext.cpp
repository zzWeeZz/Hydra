#include "HYpch.h"
#include "DxContext.h"
#include <Hydra/API/DX12/Backend/DxPhysicalDevice.h>
#include <Hydra/API/DX12/Backend/DxDevice.h>
#include <Hydra/API/DX12/Backend/DxSwapchain.h>
#include <Hydra/API/DX12/DxAllocator.h>
namespace Hydra
{
	void DxContext::Initalize(const ContextSpecification& specs)
	{
		m_Specs = specs;

		PhysicalDeviceSpecifications pdSpecs = {};
		pdSpecs.queueTypes = m_Specs.queueTypes;

		auto dxPhysicalDevice = std::make_shared<DxPhysicalDevice>(pdSpecs);
		m_PhysicalDevice = std::reinterpret_pointer_cast<PhysicalDevice>(dxPhysicalDevice);

		m_Device = std::make_shared<DxDevice>(m_PhysicalDevice);

		auto dxDevice = std::reinterpret_pointer_cast<DxDevice>(m_Device);
		dxDevice->CreateCommandLists(dxPhysicalDevice);

		DxAllocator::Initialize(dxPhysicalDevice->GetAdapter().Get(), dxDevice->Get());

		SwapchainSpecfications scSpecs = {};
		scSpecs.context = shared_from_this();
		m_Swapchain = std::make_shared<DxSwapchain>(scSpecs);




	}
	void DxContext::Shutdown()
	{
		HY_CORE_INFO("Dx12: Shutdown protocall initalized...");
		DxAllocator::Flush();
		DxAllocator::Shutdown();
		HY_CORE_INFO("Dx12: Shutdown successful!");
	}
	void DxContext::WaitForIdle()
	{
		auto dxSwapchain = std::reinterpret_pointer_cast<DxSwapchain>(m_Swapchain);

		dxSwapchain->WaitOnAllFences();
	}
}
