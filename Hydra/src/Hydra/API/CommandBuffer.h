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

		virtual void ClearFramebuffer(Ref<Framebuffer>& framebuffer, float color[4]){}
		virtual void BeginFramebuffer(Ref<Framebuffer>& framebuffer){}
		virtual void EndFramebuffer(Ref<Framebuffer>& framebuffer){}

		virtual void BindPipeline(Ref<Pipeline>& pipeline){}
		virtual void UnbindPipeline(Ref<Pipeline>& pipeline){}

		virtual void DrawInstanced(){}
		virtual void DrawIndexedInstanced(){}

		virtual ~CommandBuffer() {}

	protected:
		CommandBufferSpecification m_Specs;
	};
}