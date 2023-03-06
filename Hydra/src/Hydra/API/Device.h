#pragma once
#include "Hydra/API/PhysicalDevice.h"
#include "Hydra/Utils/Memory.h"

namespace Hydra
{
	class Device
	{
	public:
		Device(Ptr<PhysicalDevice> physicalDevice) {}
		virtual ~Device(){}

	protected:
	};
}