#pragma once
#include "Hydra/API/DeviceQueue.h"
#include "Hydra/API/DX12/DxUtils.h"
namespace Hydra
{
	class DxDeviceQueue : public DeviceQueue
	{
	public:
		DxDeviceQueue() = default;
		DxDeviceQueue(ID3D12Device2* device, D3D12_COMMAND_QUEUE_DESC& desc);

		[[nodiscard]] FORCEINLINE ID3D12CommandQueue* Get() { return m_Handle.Get(); }
		[[nodiscard]] FORCEINLINE ID3D12CommandQueue* Get() const { return m_Handle.Get(); }
		[[nodiscard]] FORCEINLINE ID3D12CommandQueue** GetAddressOf() { return m_Handle.GetAddressOf(); }
	private:
		WinRef<ID3D12CommandQueue> m_Handle;
	};
}
