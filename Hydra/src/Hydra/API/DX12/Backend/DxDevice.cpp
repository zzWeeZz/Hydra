#include "HYpch.h"
#include "DxDevice.h"
#include <Hydra/API/DX12/Backend/DxPhysicalDevice.h>
#include <Hydra/API/DX12/DxDeviceQueue.h>
#include "Hydra/API/DX12/CommandSubmiting/DxCommandBuffer.h"
#include "Hydra/API/DX12/CommandSubmiting/DxCommandQueue.h"
#include <Hydra/API/DX12/Resources/DxFramebuffer.h>
#include <Hydra/API/DX12/Pipeline/DxGraphicsPipeline.h>
#include <Hydra/API/DX12/Resources/DxBuffer.h>
namespace Hydra
{
	DxDevice::DxDevice(Ptr<PhysicalDevice> physicalDevice) : Device(physicalDevice)
	{
		Create(physicalDevice);
	}
	void DxDevice::Create(Ptr<PhysicalDevice> phycicalDevice)
	{
		auto dxPhyiscalDevice = std::reinterpret_pointer_cast<DxPhysicalDevice>(phycicalDevice.lock());

		WinRef<ID3D12Debug> spDebugController;
		HY_DX_CHECK(D3D12GetDebugInterface(HY_DX_ID(spDebugController)));
		spDebugController->EnableDebugLayer();

		const int32_t adapterIndex = dxPhyiscalDevice->GetAdapterIndex();
		const auto adapter = dxPhyiscalDevice->GetAdapter();

		HRESULT hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, HY_DX_ID(m_Device));

		if (FAILED(hr))
		{
			HY_CORE_ERROR("DX12: Failed to create device!");
			return;
		}
		HY_CORE_INFO("DX12: Successfully created device!");
		
		m_Device->QueryInterface(m_InfoQueue.GetAddressOf());


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
		m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, false);

	}
	void DxDevice::CreateFramebuffer(FramebufferSpecification& frameBufferSpecs, Ref<Framebuffer>& framebuffer)
	{
		auto dxFramebuffer = std::make_shared<DxFramebuffer>(frameBufferSpecs, shared_from_this());
		framebuffer = std::move(dxFramebuffer);
	}
	void DxDevice::DestroyFramebuffer(Ref<Framebuffer> framebuffer)
	{
		framebuffer.reset();
	}
	void DxDevice::CreateBuffer(BufferCreateSpecification& bufferSpecs, Ref<Buffer>& buffer)
	{
		buffer = std::make_shared<DxBuffer>(bufferSpecs, std::reinterpret_pointer_cast<DxDevice>(shared_from_this()));
	}
	void DxDevice::CreateGraphicsPipeline(GraphicsPipelineSpecification& pipelineSpecs, Ref<GraphicsPipeline>& graphicsPipeline)
	{
		graphicsPipeline = std::make_shared<DxGraphicsPipeline>(pipelineSpecs, std::reinterpret_pointer_cast<DxDevice>(shared_from_this()));
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
	void DxDevice::UpdateValidationLayer()
	{
		if (!m_InfoQueue) return;

		SIZE_T messageLength = 0;
		auto gotMessage = m_InfoQueue->GetMessage(0, NULL, &messageLength);

		// Allocate space and get the message
		if (messageLength)
		{
			D3D12_MESSAGE* pMessage = nullptr;
			pMessage = (D3D12_MESSAGE*)malloc(messageLength);
			gotMessage = m_InfoQueue->GetMessage(0, pMessage, &messageLength);

			switch (pMessage->Severity)
			{
			case D3D12_MESSAGE_SEVERITY_INFO:
				HY_CORE_INFO(pMessage->pDescription);
				break;
			case D3D12_MESSAGE_SEVERITY_WARNING:
				HY_CORE_WARN(pMessage->pDescription);
				break;
			case D3D12_MESSAGE_SEVERITY_ERROR:
				HY_CORE_ERROR(pMessage->pDescription);
				break;
			case D3D12_MESSAGE_SEVERITY_MESSAGE:
				HY_CORE_TRACE(pMessage->pDescription);
				break;
			default:
				break;
			}

			free(pMessage);
		}
	}
}