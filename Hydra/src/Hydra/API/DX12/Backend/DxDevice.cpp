#include "HYpch.h"
#include "DxDevice.h"
#include <Hydra/API/DX12/Backend/DxPhysicalDevice.h>
#include <Hydra/API/DX12/DxDeviceQueue.h>
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
		auto& physicalDeviceSpecs = dxPhyiscalDevice->GetSpecifications();

		D3D12_COMMAND_QUEUE_DESC cqDesc = {};

		if (physicalDeviceSpecs.queueTypes & QueueType::Graphics)
		{
			cqDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			m_DeviceQueues[QueueType::Graphics] = std::make_shared<DxDeviceQueue>(m_Device.Get(), cqDesc);
		}
		if (physicalDeviceSpecs.queueTypes & QueueType::Transfer)
		{
			cqDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
			m_DeviceQueues[QueueType::Transfer] = std::make_shared<DxDeviceQueue>(m_Device.Get(), cqDesc);
		}
		if (physicalDeviceSpecs.queueTypes & QueueType::Compute)
		{
			cqDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
			m_DeviceQueues[QueueType::Compute] = std::make_shared<DxDeviceQueue>(m_Device.Get(), cqDesc);
		}
	}
}