#include "HYpch.h"
#include "Renderer.h"
#include "Hydra/API/Framebuffer.h"
#include "Hydra/Rendering/GraphicsContext.h"
#include "Hydra/API/Device.h"
#include "Hydra/API/ShaderCompiler.h"
#include "Hydra/API/Factory.h"
#include "Hydra/API/ResourceInterface/Shader.h"
#include "Hydra/API/PipelineInterface/GraphicsPipeline.h"
#include "Hydra/API/ResourceInterface/Buffer.h"
#include "Hydra/Events/ApplicationEvent.h"

namespace Hydra
{
	struct Cache
	{
		Ref<Framebuffer> framebuffer;
	};

	static Ref<Cache> cache;
	static Ref<ShaderCompiler> shaderCompiler;

	static Ref<Shader> testShader;
	static Ref<Shader> testShader2;
	static Ref<GraphicsPipeline> testPipeline;
	static Ref<GraphicsPipeline> testPipeline2;

	static Ref<Buffer> testVertexBuffer;
	static Ref<Buffer> testIndexBuffer;


	struct Vertex
	{
		float position[4];
	};


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

		testShaderSpecs.vertexShaderStage.filepath = "Shaders/Triangle2_vs.hlsl";
		testShaderSpecs.pixelShaderStage.filepath = "Shaders/Triangle_ps.hlsl";
		std::string str;
		shaderCompiler->Compile(testShaderSpecs, testShader, str);



		GraphicsPipelineSpecification graphicsSpecs = {};

		graphicsSpecs.shaderObject = testShader;
		graphicsSpecs.framebufferObject = cache->framebuffer;
		auto& attribute = graphicsSpecs.vertexBindingAttributes.emplace_back();
		attribute.binding = 0;
		attribute.location = 0;
		attribute.format = ImageFormat::RGBA32F;
		attribute.offset = 0;

		graphicsSpecs.bindingDescription.binding = 0;
		graphicsSpecs.bindingDescription.stride = sizeof(Vertex);

		GraphicsContext::GetDevice().lock()->CreateGraphicsPipeline(graphicsSpecs, testPipeline);
		
		
		std::array<Vertex, 4> verts = {};

		verts[0].position[0] = -0.5f;
		verts[0].position[1] = -0.5f;
		verts[0].position[2] = 0.f;
		verts[0].position[3] = 1.f;

		verts[1].position[0] = 0.5f;
		verts[1].position[1] = -0.5f;
		verts[1].position[2] = 0.f;
		verts[1].position[3] = 1.f;

		verts[2].position[0] = 0.5f;
		verts[2].position[1] = 0.5f;
		verts[2].position[2] = 0.f;
		verts[2].position[3] = 1.f;

		verts[3].position[0] = -0.5f;
		verts[3].position[1] = 0.5f;
		verts[3].position[2] = 0.f;
		verts[3].position[3] = 1.f;

		std::array<uint16_t, 6> indices = {0, 1, 2, 2, 3, 0};

		BufferCreateSpecification bufferSpecs = {};

		bufferSpecs.usage = BufferUsage::VertexBuffer;
		bufferSpecs.stride = sizeof(Vertex);
		bufferSpecs.size = verts.size();
		bufferSpecs.data = verts.data();
		bufferSpecs.allocationUsage = MemoryUsage::CPU_To_GPU;

		GraphicsContext::GetDevice().lock()->CreateBuffer(bufferSpecs, testVertexBuffer);

		bufferSpecs.usage = BufferUsage::IndexBuffer;
		bufferSpecs.stride = sizeof(uint32_t);
		bufferSpecs.size = indices.size();
		bufferSpecs.data = indices.data();
		bufferSpecs.allocationUsage = MemoryUsage::CPU_To_GPU;

		GraphicsContext::GetDevice().lock()->CreateBuffer(bufferSpecs, testIndexBuffer);
	}
	void Renderer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>([&] (WindowResizeEvent& e)
			{
				cache->framebuffer->Resize(e.GetWidth(), e.GetHeight());
			});
	}
	void Renderer::Begin()
	{
		auto swapchain = GraphicsContext::GetSwapchain().lock();

		auto frameIndex = swapchain->PrepareNewFrame();

		if (frameIndex < 0)
		{
			// just skip this frame.
			return;
		}

		auto commandQueue = GraphicsContext::GetDevice().lock()->GetCommandQueue(QueueType::Graphics, frameIndex).lock();
		auto commandBuffer = commandQueue->GetCommandBuffer();

		commandQueue->Reset();
		commandBuffer.lock()->Begin();

		float clear[] = { 0.32f, 0.32f, 0.32f, 1.f };

		commandBuffer.lock()->BeginFramebuffer(frameIndex, cache->framebuffer, clear);


		commandBuffer.lock()->BindGraphicsPipeline(frameIndex, testPipeline);

		commandBuffer.lock()->BindVertexBuffer(frameIndex, testVertexBuffer);
		commandBuffer.lock()->BindIndexBuffer(frameIndex, testIndexBuffer);

		commandBuffer.lock()->DrawIndexedInstanced(6, 1, 0, 0, 0);


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