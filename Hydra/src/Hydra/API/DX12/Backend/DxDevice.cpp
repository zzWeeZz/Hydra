#include "HYpch.h"
#include "DxDevice.h"
#include <Hydra/API/DX12/Backend/DxPhysicalDevice.h>
#include <Hydra/API/DX12/DxDeviceQueue.h>
#include "Hydra/API/DX12/CommandSubmiting/DxCommandBuffer.h"
#include "Hydra/API/DX12/CommandSubmiting/DxCommandQueue.h"
#include <Hydra/API/DX12/Resources/DxFramebuffer.h>
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
	void DxDevice::CreateFramebuffer(FramebufferSpecification& frameBufferSpecs, Ref<Framebuffer>& framebuffer)
	{
		auto dxFramebuffer = std::make_shared<DxFramebuffer>(frameBufferSpecs, shared_from_this());
		framebuffer = std::move(dxFramebuffer);
	}
	void DxDevice::CreateCommandLists(Ptr<DxPhysicalDevice> physicalDevice)
	{
		auto& physicalDeviceSpecs = physicalDevice.lock()->GetSpecifications();
		if (physicalDeviceSpecs.queueTypes & QueueType::Graphics)
		{
			m_CommandQueues[QueueType::Graphics] = {};
		}
		if (physicalDeviceSpecs.queueTypes & QueueType::Transfer)
		{
			m_CommandQueues[QueueType::Transfer] = {};
		}
		if (physicalDeviceSpecs.queueTypes & QueueType::Compute)
		{
			m_CommandQueues[QueueType::Compute] = {};
		}

		for (auto& [queueType, commandQueues] : m_CommandQueues)
		{
			auto dxQueue = std::reinterpret_pointer_cast<DxDeviceQueue>(m_DeviceQueues[queueType]);
			for (size_t i = 0; i < g_FramesInFlight; ++i)
			{
				commandQueues[i] = std::make_shared<DxCommandQueue>();
				std::reinterpret_pointer_cast<DxCommandQueue>(commandQueues[i])->Create(std::reinterpret_pointer_cast<DxDevice>(shared_from_this()), dxQueue);
			}
		}
	}
}