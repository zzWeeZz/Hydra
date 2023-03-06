#pragma once
#include "Hydra/API/PhysicalDevice.h"
#include "Hydra/API/DX12/DxUtils.h"
namespace Hydra
{
	class DxPhysicalDevice : public PhysicalDevice
	{
	public:
		DxPhysicalDevice(const PhysicalDeviceSpecifications& specs);
		// Inherited via PhysicalDevice
		void Create() override;
		[[nodiscard]] WinRef<IDXGIAdapter1> GetAdapter() { return m_Adapter; }
		[[nodiscard]] WinRef<IDXGIFactory4> GetFactory() { return m_Factory; }
		[[nodiscard]] int32_t GetAdapterIndex() const { return m_AdapterIndex; }
	private:
		int32_t m_AdapterIndex = 0;
		WinRef<IDXGIFactory4> m_Factory;
		WinRef<IDXGIAdapter1> m_Adapter; // this is the Physical device of Dx12.
	};
}
