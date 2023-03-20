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
		
		void CreateFramebuffer(FramebufferSpecification& frameBufferSpecs, Ref<Framebuffer>& framebuffer) override;
		void DestroyFramebuffer(Ref<Framebuffer> framebuffer) override;

		void CreateGraphicsPipeline(GraphicsPipelineSpecification& pipelineSpecs, Ref<GraphicsPipeline>& graphicsPipeline) override;

		[[nodiscard]] FORCEINLINE ID3D12Device2* Get() { return m_Device.Get(); }
		[[nodiscard]] FORCEINLINE ID3D12Device2* Get() const { return m_Device.Get(); }
		[[nodiscard]] FORCEINLINE ID3D12Device2** GetAddressOf() { return m_Device.GetAddressOf(); }

		void CreateCommandLists(Ptr<DxPhysicalDevice> physicalDevice);
		
		void UpdateValidationLayer();

	private:

		WinRef<ID3D12InfoQueue> m_InfoQueue;
		WinRef<ID3D12Device2> m_Device;

		// Inherited via Device
	};
}
