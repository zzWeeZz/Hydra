#include "HYpch.h"
#include "DxDevice.h"
#include <Hydra/API/DX12/Backend/DxPhysicalDevice.h>
namespace Hydra
{
	DxDevice::DxDevice(Ptr<PhysicalDevice> physicalDevice) : Device(physicalDevice)
	{
		Create(physicalDevice);
	}
	void DxDevice::Create(Ptr<PhysicalDevice> phycicalDevice)
	{
		auto dxPhyiscalDevice = std::reinterpret_pointer_cast<DxPhysicalDevice>(phycicalDevice.lock());

		const int32_t adapterIndex = dxPhyiscalDevice->GetAdapterIndex();
		const auto adapter = dxPhyiscalDevice->GetAdapter();

		HRESULT hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, HY_DX_ID(m_Device));

		if (FAILED(hr))
		{
			HY_CORE_ERROR("DX12: Failed to create device!");
			return;
		}
		HY_CORE_INFO("DX12: Successfully created device!");
		D3D12_COMMAND_QUEUE_DESC cqDesc = {};

		hr = m_Device->CreateCommandQueue(&cqDesc, HY_DX_ID(m_CommandQueue));
		if (FAILED(hr))
		{
			HY_CORE_ERROR("DX12: Failed to create command queue!");
			return;
		}
		HY_CORE_INFO("DX12: Successfully created command queue!");
	}
}