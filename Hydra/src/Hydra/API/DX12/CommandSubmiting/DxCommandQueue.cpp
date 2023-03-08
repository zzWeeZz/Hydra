#include "HYpch.h"
#include "DxCommandQueue.h"
#include "Hydra/API/DX12/DxDeviceQueue.h"

#include "Hydra/API/DX12/Backend/DxDevice.h"
#include <Hydra/API/DX12/CommandSubmiting/DxCommandBuffer.h>

namespace Hydra
{
	void DxCommandQueue::Create(Ptr<DxDevice> device, Ptr<DxDeviceQueue> deviceQueue)
	{
		D3D12_COMMAND_LIST_TYPE commandType = {};
		m_Queue = deviceQueue;
		switch (deviceQueue.lock()->GetType())
		{
		case QueueType::Graphics:
			commandType = D3D12_COMMAND_LIST_TYPE_DIRECT;
			break;
		case QueueType::Transfer:
			commandType = D3D12_COMMAND_LIST_TYPE_COPY;
			break;
		case QueueType::Compute:
			commandType = D3D12_COMMAND_LIST_TYPE_COMPUTE;
			break;
		default:
			break;
		}

		HY_DX_CHECK(device.lock()->Get()->CreateCommandAllocator(commandType, HY_DX_ID(m_CommandAllocator)));
		m_CommandBuffer = AllocateCommandBuffer(device, CommandBufferLevel::Primary);
	}
	Ref<CommandBuffer> DxCommandQueue::AllocateCommandBuffer(Ptr<Device> device, CommandBufferLevel level)
	{
		CommandBufferSpecification commandBufferSpecs = {};
		commandBufferSpecs.device = device;
		commandBufferSpecs.queue = std::reinterpret_pointer_cast<DxCommandQueue>(shared_from_this());
		commandBufferSpecs.level = level;
		return std::make_shared<DxCommandBuffer>(commandBufferSpecs);
	}
}
