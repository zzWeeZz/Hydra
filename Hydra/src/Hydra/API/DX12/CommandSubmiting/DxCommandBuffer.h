#pragma once
#include "Hydra/API/CommandBuffer.h"
#include "Hydra/API/DX12/DxUtils.h"
namespace Hydra
{
	class DxCommandBuffer : public CommandBuffer
	{
	public:
		DxCommandBuffer(CommandBufferSpecification& specs);

		void Begin() override;
		void End() override;

		void BeginFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer, float color[4]) override;
		void EndFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer) override;

		void BindVertexBuffer(uint32_t frameindex, Ref<Buffer>& buffer) override;
		void BindIndexBuffer(uint32_t frameindex, Ref<Buffer>& buffer) override;
		void BindConstantBuffer(uint32_t frameindex, uint32_t bindPoint, uint32_t space, Ref<Buffer>& buffer) override;

		void BindGraphicsPipeline(uint32_t frameIndex, Ref<GraphicsPipeline>& pipeline) override;

		void DrawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override;
		void DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) override;

		void CopyFramebufferToSwapchain(uint32_t frameIndex, Ref<Framebuffer>& framebuffer, Ref<Swapchain> swapchain) override;

		[[nodiscard]] FORCEINLINE ID3D12CommandList* GetList() { return reinterpret_cast<ID3D12CommandList*>(m_CommandList.Get()); }
		[[nodiscard]] FORCEINLINE ID3D12CommandList** GetListAddress() { return reinterpret_cast<ID3D12CommandList**>(m_CommandList.GetAddressOf()); }

		[[nodiscard]] FORCEINLINE ID3D12GraphicsCommandList* Get() { return m_CommandList.Get(); }
		[[nodiscard]] FORCEINLINE ID3D12GraphicsCommandList* Get() const { return m_CommandList.Get(); }
		[[nodiscard]] FORCEINLINE ID3D12GraphicsCommandList** GetAddressOf() { return m_CommandList.GetAddressOf(); }

	private:
		WinRef<ID3D12GraphicsCommandList> m_CommandList;
	};
}
