#pragma once
#include "Hydra/API/Device.h"
#include "Hydra/API/DX12/DxUtils.h"

namespace Hydra
{
	class DxPhysicalDevice;
	class DxDevice : public Device
	{
	public:
		DxDevice(Ptr<PhysicalDevice> physicalDevice);
		// Inherited via Device
		void Create(Ptr<PhysicalDevice> phycicalDevice);
		
		[[nodiscard]] FORCEINLINE ID3D12Device2* Get() { return m_Device.Get(); }
		[[nodiscard]] FORCEINLINE ID3D12Device2* Get() const { return m_Device.Get(); }
		[[nodiscard]] FORCEINLINE ID3D12Device2** GetAddressOf() { return m_Device.GetAddressOf(); }

		void CreateCommandLists(Ptr<DxPhysicalDevice> physicalDevice);
		 
	private:


		WinRef<ID3D12Device2> m_Device;
	};
}
