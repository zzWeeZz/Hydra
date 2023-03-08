#pragma once
#include "Hydra/API/PhysicalDevice.h"
#include "Hydra/Utils/Memory.h"
#include <Hydra/API/DeviceQueue.h>
#include <Hydra/API/CommandBuffer.h>
#include <Hydra/API/CommandQueue.h>
namespace Hydra
{
	class Device
	{
	public:
		Device(Ptr<PhysicalDevice> physicalDevice) {}
		virtual ~Device(){}

	 	[[nodiscard]] FORCEINLINE Ptr<DeviceQueue> GetQueue(QueueType type) { return m_DeviceQueues[type]; }
		[[nodiscard]] FORCEINLINE Ptr<CommandQueue> GetCommandQueue(QueueType queueType) { return m_CommandQueues[queueType]; }

	protected:
		std::unordered_map<QueueType, Ref<DeviceQueue>> m_DeviceQueues;
		std::unordered_map<QueueType, Ref<CommandQueue>> m_CommandQueues;
	};
}