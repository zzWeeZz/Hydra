#pragma once
#include "Hydra/API/DeviceQueue.h"
namespace Hydra
{
	struct PhysicalDeviceSpecifications
	{
		bool EnableDebugging = true;
		QueueType queueTypes;
	};

	class PhysicalDevice
	{
	public:
		PhysicalDevice(const PhysicalDeviceSpecifications& specs) { m_Specs = specs; };

		[[nodiscard]] FORCEINLINE PhysicalDeviceSpecifications& GetSpecifications() { return m_Specs; }

	protected:
		PhysicalDeviceSpecifications m_Specs;
	};
}