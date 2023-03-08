#pragma once
#include "Hydra/Utils/Memory.h"
#include <Hydra/API/DeviceQueue.h>
#include "Hydra/API/CommandBuffer.h"
namespace Hydra
{
	struct CommandQueueFetchData
	{
		Ptr<DeviceQueue> deviceQueue;
	};

	class CommandQueue
	{
	public:
		CommandQueue() = default;
		virtual ~CommandQueue(){}
		virtual Ref<CommandBuffer> AllocateCommandBuffer(Ptr<Device> device, CommandBufferLevel level) = 0;

		inline void FetchCommandQueueData(CommandQueueFetchData& outFetchData)
		{
			outFetchData.deviceQueue = m_Queue;
		}

	protected:
		Ptr<DeviceQueue> m_Queue;
	};
}