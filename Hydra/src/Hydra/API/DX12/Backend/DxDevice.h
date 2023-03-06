#pragma once
#include "Hydra/API/Device.h"
#include "Hydra/API/DX12/DxUtils.h"
namespace Hydra
{
	class DxDevice : public Device
	{
	public:
		DxDevice(Ptr<PhysicalDevice> physicalDevice);
		// Inherited via Device
		void Create(Ptr<PhysicalDevice> phycicalDevice);

		[[nodiscard]] WinRef<ID3D12CommandQueue> GetCommandQueue() { return m_CommandQueue; }

	private:
		WinRef<ID3D12Device2> m_Device;
		WinRef<ID3D12CommandQueue> m_CommandQueue;
	};
}
