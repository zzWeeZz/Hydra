#include "HYpch.h"
#include "Renderer.h"
#include "Hydra/API/Framebuffer.h"
#include "Hydra/Rendering/GraphicsContext.h"
#include "Hydra/API/Device.h"
namespace Hydra
{
	struct Cache
	{
		Ref<Framebuffer> framebuffer;
	};

	static Ref<Cache> cache;

	void Renderer::Initialize()
	{
		cache = std::make_shared<Cache>();
		auto device = GraphicsContext::GetDevice();
		FramebufferSpecification framebufferSpecs = {};
		framebufferSpecs.formats.emplace_back(ImageFormat::RGBA8UN);
		framebufferSpecs.width = 1280;
		framebufferSpecs.height = 720;

		device.lock()->CreateFramebuffer(framebufferSpecs, cache->framebuffer);
	}
	void Renderer::Begin()
	{
		auto swapchain = GraphicsContext::GetSwapchain().lock();
		auto commandQueue = GraphicsContext::GetDevice().lock()->GetCommandQueue(QueueType::Graphics).lock();
		swapchain->PrepareNewFrame();
		commandQueue->Reset();


		commandQueue->Submit(swapchain);
		swapchain->Present();
	}
	void Renderer::Shutdown()
	{
		auto device = GraphicsContext::GetDevice();
		device.lock()->DestroyFramebuffer(cache->framebuffer);
	}
}