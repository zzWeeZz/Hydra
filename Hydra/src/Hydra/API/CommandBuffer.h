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

	class CommandBuffer
	{
	public:
		CommandBuffer() = default;
		CommandBuffer(CommandBufferSpecification& specs) { m_Specs = specs; }
		virtual ~CommandBuffer() {}

	protected:
		CommandBufferSpecification m_Specs;
	};
}