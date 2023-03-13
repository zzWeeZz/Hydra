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
	class Pipeline;

	class CommandBuffer
	{
	public:
		CommandBuffer() = default;
		CommandBuffer(CommandBufferSpecification& specs) { m_Specs = specs; }

		virtual void Begin(){}
		virtual void End(){}

		virtual void ClearFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer, float color[4]){}
		virtual void BeginFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer){}
		virtual void EndFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer){}

		virtual void BindPipeline(uint32_t frameIndex, Ref<Pipeline>& pipeline){}
		virtual void UnbindPipeline(uint32_t frameIndex, Ref<Pipeline>& pipeline){}

		virtual void DrawInstanced(){}
		virtual void DrawIndexedInstanced(){}

		virtual ~CommandBuffer() {}

	protected:
		CommandBufferSpecification m_Specs;
	};
}