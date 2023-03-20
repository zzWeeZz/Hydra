#pragma once

#include "Hydra/Utils/Memory.h"

namespace Hydra
{
	class CommandQueue;
	class Device;
	enum class CommandBufferLevel
	{
		Primary,
		Secondary,
	};

	struct CommandBufferSpecification
	{
		Ptr<Device> device;
		Ptr<CommandQueue> queue;
		CommandBufferLevel level;
	};

	class Framebuffer;
	class GraphicsPipeline;
	class Swapchain;

	class CommandBuffer
	{
	public:
		CommandBuffer() = default;
		CommandBuffer(CommandBufferSpecification& specs) { m_Specs = specs; }

		virtual void Begin(){}
		virtual void End(){}

		virtual void ClearFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer, float color[4]){}
		virtual void BeginFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer, float color[4]){}
		virtual void EndFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer){}


		virtual void BindGraphicsPipeline(uint32_t frameIndex, Ref<GraphicsPipeline>& pipeline){}
		virtual void UnbindGraphicsPipeline(uint32_t frameIndex, Ref<GraphicsPipeline>& pipeline){}

		virtual void CopyFramebufferToSwapchain(uint32_t frameIndex, Ref<Framebuffer>& framebuffer, Ref<Swapchain> swapchain){}

		virtual void DrawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance){}
		virtual void DrawIndexedInstanced(){}

		virtual ~CommandBuffer() {}

	protected:
		CommandBufferSpecification m_Specs;
	};
}