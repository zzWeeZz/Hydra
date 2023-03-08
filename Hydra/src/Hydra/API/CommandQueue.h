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

	class CommandQueue : public std::enable_shared_from_this<CommandQueue>
	{
	public:
		CommandQueue() = default;
		virtual ~CommandQueue(){}
		virtual Ref<CommandBuffer> AllocateCommandBuffer(Ptr<Device> device, CommandBufferLevel level) = 0;

		[[nodiscard]] FORCEINLINE Ptr<CommandBuffer> GetCommandBuffer() { return m_CommandBuffer; }
 
		inline void FetchCommandQueueData(CommandQueueFetchData& outFetchData)
		{
			outFetchData.deviceQueue = m_Queue;
		}

	protected:
		Ptr<DeviceQueue> m_Queue;
		Ref<CommandBuffer> m_CommandBuffer;
	};
}