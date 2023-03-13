#pragma once
#include "Hydra/API/PhysicalDevice.h"
#include "Hydra/Utils/Memory.h"
#include <Hydra/API/DeviceQueue.h>
#include <Hydra/API/CommandBuffer.h>
#include <Hydra/API/CommandQueue.h>
#include "Hydra/API/ApiUtils.h"
#include <unordered_map>
namespace Hydra
{
	struct FramebufferSpecification;
	class Framebuffer;
	class Device : public std::enable_shared_from_this<Device>
	{
	public:
		Device(Ptr<PhysicalDevice> physicalDevice) {}
		virtual ~Device(){}

		virtual void CreateFramebuffer(FramebufferSpecification& frameBufferSpecs, Ref<Framebuffer>& framebuffer){}

		virtual void DestroyFramebuffer(Ref<Framebuffer> framebuffer){}

	 	[[nodiscard]] FORCEINLINE Ptr<DeviceQueue> GetQueue(QueueType type) { return m_DeviceQueues[type]; }
		[[nodiscard]] FORCEINLINE Ptr<CommandQueue> GetCommandQueue(QueueType queueType, uint32_t frameIndex) { return m_CommandQueues[queueType][frameIndex]; } // TODO_Niklas: Fix Indexing for perframe in flight.
		[[nodiscard]] FORCEINLINE Ptr<CommandBuffer> GetCommandBuffer(QueueType queueType, uint32_t frameIndex) { return m_CommandQueues[queueType][frameIndex]->GetCommandBuffer(); } // TODO_Niklas: Fix Indexing for perframe in flight.

	protected:
		std::unordered_map<QueueType, Ref<DeviceQueue>> m_DeviceQueues;
		std::unordered_map<QueueType, PerFrameInFlight<Ref<CommandQueue>>> m_CommandQueues;
	};
}