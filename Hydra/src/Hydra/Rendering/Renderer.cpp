#include "HYpch.h"
#include "Renderer.h"
#include "Hydra/API/Framebuffer.h"
#include "Hydra/Rendering/GraphicsContext.h"
#include "Hydra/API/Device.h"
#include "Hydra/API/ShaderCompiler.h"
#include "Hydra/API/Factory.h"
#include "Hydra/API/ResourceInterface/Shader.h"
#include "Hydra/API/PipelineInterface/GraphicsPipeline.h"
namespace Hydra
{
	struct Cache
	{
		Ref<Framebuffer> framebuffer;
	};

	static Ref<Cache> cache;
	static Ref<ShaderCompiler> shaderCompiler;

	static Ref<Shader> testShader;
	static Ref<GraphicsPipeline> testPipeline;

	void Renderer::Initialize()
	{
		cache = std::make_shared<Cache>();
		auto device = GraphicsContext::GetDevice();
		FramebufferSpecification framebufferSpecs = {};
		framebufferSpecs.formats.emplace_back(ImageFormat::RGBA8UN);
		framebufferSpecs.width = 1280;
		framebufferSpecs.height = 720;

		device.lock()->CreateFramebuffer(framebufferSpecs, cache->framebuffer);
		ShaderCompilerCreateSpecification createSpecs = {};
		createSpecs.optimazationMode = OptimazationMode::Debug;
		createSpecs.behaviorFlag = CompilerBehaviorFlag::TreatWarningsAsErrors;
		Factory::ConstructShaderCompiler(createSpecs, shaderCompiler);


		ShaderSpecification testShaderSpecs = {};

		testShaderSpecs.vertexShaderStage.filepath = "Shaders/Triangle_vs.hlsl";
		testShaderSpecs.pixelShaderStage.filepath = "Shaders/Triangle_ps.hlsl";
		std::string str;
		shaderCompiler->Compile(testShaderSpecs, testShader, str);

		GraphicsPipelineSpecification graphicsSpecs = {};

		graphicsSpecs.shaderObject = testShader;
		graphicsSpecs.framebufferObject = cache->framebuffer;

		GraphicsContext::GetDevice().lock()->CreateGraphicsPipeline(graphicsSpecs, testPipeline);

	}
	void Renderer::Begin()
	{
		auto swapchain = GraphicsContext::GetSwapchain().lock();

		auto frameIndex = swapchain->PrepareNewFrame();

		auto commandQueue = GraphicsContext::GetDevice().lock()->GetCommandQueue(QueueType::Graphics, frameIndex).lock();
		auto commandBuffer = commandQueue->GetCommandBuffer();
		commandQueue->Reset();
		commandBuffer.lock()->Begin();

		float clear[] = { 0.32f, 0.32f, 0.32f, 1.f };

		commandBuffer.lock()->BeginFramebuffer(frameIndex, cache->framebuffer, clear);

		commandBuffer.lock()->BindGraphicsPipeline(frameIndex, testPipeline);
		commandBuffer.lock()->DrawInstanced(3, 1, 0, 0);

		commandBuffer.lock()->EndFramebuffer(frameIndex, cache->framebuffer);

		commandBuffer.lock()->CopyFramebufferToSwapchain(frameIndex, cache->framebuffer, GraphicsContext::GetSwapchain().lock());

		commandBuffer.lock()->End();
		commandQueue->Submit(swapchain);
		swapchain->Present();
	}
	void Renderer::Shutdown()
	{
		
		auto device = GraphicsContext::GetDevice();

		GraphicsContext::GetContext().lock()->WaitForIdle();

		device.lock()->DestroyFramebuffer(cache->framebuffer);
	}
}