#pragma once
namespace Hydra
{
	struct PhysicalDeviceSpecifications
	{
		bool EnableDebugging = true;
	};

	class PhysicalDevice
	{
	public:
		PhysicalDevice(const PhysicalDeviceSpecifications& specs) { m_Specs = specs; };

	protected:
		PhysicalDeviceSpecifications m_Specs;
	};
}