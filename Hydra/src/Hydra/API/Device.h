#pragma once
#include "Hydra/API/PhysicalDevice.h"
#include "Hydra/Utils/Memory.h"
#include <Hydra/API/DeviceQueue.h>

namespace Hydra
{
	class Device
	{
	public:
		Device(Ptr<PhysicalDevice> physicalDevice) {}
		virtual ~Device(){}

	protected:
		std::unordered_map<QueueType, Ref<DeviceQueue>> m_DeviceQueues;
	};
}