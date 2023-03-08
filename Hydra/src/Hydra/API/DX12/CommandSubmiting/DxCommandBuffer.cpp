#include "HYpch.h"
#include "DxCommandBuffer.h"
#include <Hydra/API/DX12/Backend/DxDevice.h>
#include <Hydra/API/DX12/CommandSubmiting/DxCommandQueue.h>
namespace Hydra
{
	DxCommandBuffer::DxCommandBuffer(CommandBufferSpecification& specs)
	{
		auto dxDevice = std::reinterpret_pointer_cast<DxDevice>(specs.device.lock());
		auto dxCommandQueue = std::reinterpret_pointer_cast<DxCommandQueue>(specs.queue.lock());

		CommandQueueFetchData fetchData{};

		dxCommandQueue->FetchCommandQueueData(fetchData);
		D3D12_COMMAND_LIST_TYPE commandType = {};

		switch (fetchData.deviceQueue.lock()->GetType())
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

		HY_DX_CHECK(dxDevice->Get()->CreateCommandList(0, commandType, dxCommandQueue->Get(), nullptr, HY_DX_ID(m_CommandList)));
	}
}