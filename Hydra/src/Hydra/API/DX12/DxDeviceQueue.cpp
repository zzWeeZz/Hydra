#include "HYpch.h"
#include "DxDeviceQueue.h"
namespace Hydra
{
	DxDeviceQueue::DxDeviceQueue(ID3D12Device2* device, D3D12_COMMAND_QUEUE_DESC& desc)
	{
		QueueType type = QueueType::Count;
		switch (desc.Type)
		{
		case D3D12_COMMAND_LIST_TYPE_DIRECT:
			type = QueueType::Graphics;
			break;
		case D3D12_COMMAND_LIST_TYPE_COMPUTE:
			type = QueueType::Compute;
			break;
		case D3D12_COMMAND_LIST_TYPE_COPY:
			type = QueueType::Transfer;
			break;
		default:
			HY_CORE_ERROR("Dx12: Invalid Command list type. Most be one of these 3: DIRECT, COMPUTE, COPY!");
			break;
		}
		if (type & QueueType::Count)
		{
			return;
		}
		HY_DX_CHECK(device->CreateCommandQueue(&desc, HY_DX_ID(m_Handle)));
		HY_CORE_INFO("DX12: Successfully created command queue!");
	}
}