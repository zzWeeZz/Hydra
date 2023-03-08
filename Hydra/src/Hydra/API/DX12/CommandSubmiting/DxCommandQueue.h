#pragma once
#include "Hydra/API/CommandQueue.h"
#include "Hydra/API/DX12/DxUtils.h"
namespace Hydra
{
	class DxDevice;
	class DxDeviceQueue;
	class DxCommandQueue : public CommandQueue
	{
	public:
		DxCommandQueue() = default;
		void Create(Ptr<DxDevice> device, Ptr<DxDeviceQueue> deviceQueue);
		[[nodiscard]] FORCEINLINE ID3D12CommandAllocator* Get() { return m_CommandAllocator.Get(); }
		[[nodiscard]] FORCEINLINE ID3D12CommandAllocator* Get() const { return m_CommandAllocator.Get(); }
		[[nodiscard]] FORCEINLINE ID3D12CommandAllocator** GetAddressOf() { return m_CommandAllocator.GetAddressOf(); }
		// Inherited via CommandQueue
		virtual Ref<CommandBuffer> AllocateCommandBuffer(Ptr<Device> device, CommandBufferLevel level) override;
	private:
		WinRef<ID3D12CommandAllocator> m_CommandAllocator;
	};
}
