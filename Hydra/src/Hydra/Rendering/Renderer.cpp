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
#include "Hydra/API/ResourceInterface/Image.h"
#include "Hydra/Events/ApplicationEvent.h"
#include "glm/glm/glm.hpp"
#include <glm/glm/gtx/quaternion.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

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

	static Ref<Buffer> testConstantBuffer;

	static Ref<Image> testImage;

	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 color = glm::vec4(1.f);
		glm::vec2 uv = glm::vec2(0.f);
	};

	struct CameraData
	{
		glm::mat4 view;
		glm::mat4 proj;
		glm::mat4 objectSpace;
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

		void* pix;
		int texWidth, texHeight, texChannels;
		pix = stbi_load("Titan.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		ImageSpecification specs = {};
		specs.ImageData = pix;
		specs.height = texHeight;
		specs.width = texWidth;

		GraphicsContext::GetDevice().lock()->CreateImage(specs, testImage);


		ShaderSpecification testShaderSpecs = {};

		testShaderSpecs.vertexShaderStage.filepath = "Shaders/Triangle2_vs.hlsl";
		testShaderSpecs.pixelShaderStage.filepath = "Shaders/Triangle_ps.hlsl";
		std::string str;
		shaderCompiler->Compile(testShaderSpecs, testShader, str);

		testShaderSpecs.pixelShaderStage.filepath = "Shaders/Triangle2_ps.hlsl";
		shaderCompiler->Compile(testShaderSpecs, testShader2, str);


		GraphicsPipelineSpecification graphicsSpecs = {};

		graphicsSpecs.shaderObject = testShader;
		graphicsSpecs.framebufferObject = cache->framebuffer;

		graphicsSpecs.bindingDescription.binding = 0;
		graphicsSpecs.bindingDescription.stride = sizeof(Vertex);

		GraphicsContext::GetDevice().lock()->CreateGraphicsPipeline(graphicsSpecs, testPipeline);

		graphicsSpecs.shaderObject = testShader2;
		GraphicsContext::GetDevice().lock()->CreateGraphicsPipeline(graphicsSpecs, testPipeline2);
		
		
		std::vector<Vertex> vertices = {
	{glm::vec4(-1.0f, -1.0f,  1.0f, 1.0f), glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec2(0.f, 0.f)}, // Front Bottom Left
	{glm::vec4(1.0f, -1.0f,  1.0f, 1.0f), glm::vec4(0.f, 1.f, 0.f, 1.f), glm::vec2(1.f, 0.f)}, // Front Bottom Right
	{glm::vec4(1.0f,  1.0f,  1.0f, 1.0f), glm::vec4(0.f, 0.f, 1.f, 1.f), glm::vec2(1.f, 1.f)}, // Front Top Right
	{glm::vec4(-1.0f,  1.0f,  1.0f, 1.0f), glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec2(0.f, 1.f)}, // Front Top Left
	{glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f), glm::vec4(1.f, 1.f, 0.f, 1.f), glm::vec2(0.f, 0.f)}, // Back Bottom Left
	{glm::vec4(1.0f, -1.0f, -1.0f, 1.0f), glm::vec4(0.f, 1.f, 1.f, 1.f), glm::vec2(1.f, 0.f)}, // Back Bottom Right
	{glm::vec4(1.0f,  1.0f, -1.0f, 1.0f), glm::vec4(1.f, 0.f, 1.f, 1.f), glm::vec2(1.f, 1.f)}, // Back Top Right
	{glm::vec4(-1.0f,  1.0f, -1.0f, 1.0f), glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec2(0.f, 1.f)}  // Back Top Left
		};

		

		std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0, // Front Face
		1, 5, 6, 6, 2, 1, // Right Face
		4, 7, 6, 6, 5, 4, // Back Face
		0, 3, 7, 7, 4, 0, // Left Face
		3, 2, 6, 6, 7, 3, // Top Face
		0, 4, 5, 5, 1, 0  // Bottom Face
		};

		BufferCreateSpecification bufferSpecs = {};

		bufferSpecs.usage = BufferUsage::VertexBuffer;
		bufferSpecs.stride = sizeof(Vertex);
		bufferSpecs.count = vertices.size();
		bufferSpecs.data = vertices.data();
		bufferSpecs.allocationUsage = MemoryUsage::CPU_To_GPU;

		GraphicsContext::GetDevice().lock()->CreateBuffer(bufferSpecs, testVertexBuffer);

		bufferSpecs.usage = BufferUsage::IndexBuffer;
		bufferSpecs.stride = sizeof(uint16_t);
		bufferSpecs.count = indices.size();
		bufferSpecs.data = indices.data();
		bufferSpecs.allocationUsage = MemoryUsage::CPU_To_GPU;

		GraphicsContext::GetDevice().lock()->CreateBuffer(bufferSpecs, testIndexBuffer);

		bufferSpecs.usage = BufferUsage::ConstantBuffer;
		bufferSpecs.stride = sizeof(CameraData);
		bufferSpecs.count = 40;
		bufferSpecs.data = nullptr;
		bufferSpecs.allocationUsage = MemoryUsage::CPU_To_GPU;

		GraphicsContext::GetDevice().lock()->CreateBuffer(bufferSpecs, testConstantBuffer);
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
		auto commandBuffer = commandQueue->GetCommandBuffer().lock();

		commandQueue->Reset();
		commandBuffer->Begin();
		static float time = 0;
		time += 0.003f;
		CameraData data = {};
		data.proj = glm::perspective(glm::radians(90.f), 16.f/9.f, 1.f, 20000.f);

		data.view = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 10));
		data.view = glm::inverse(data.view);
		

		auto pos = glm::translate(glm::mat4(1.0f), glm::vec3());
		auto rot = glm::mat4_cast(glm::tquat<float>(glm::vec3(time, time * 2, time / 2.f)));
		auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.f));
		data.objectSpace = pos * rot * scale;

		testConstantBuffer->CopyToBuffer(frameIndex, &data, sizeof(CameraData), 0);

		float clear[] = { 0.32f, 0.32f, 0.32f, 1.f };

		commandBuffer->BeginFramebuffer(frameIndex, cache->framebuffer, clear);

		commandBuffer->BindGraphicsPipeline(frameIndex, testPipeline);
		
		commandBuffer->BindConstantBuffer(frameIndex, 0, 0, testConstantBuffer, 0);

		commandBuffer->BindVertexBuffer(frameIndex, testVertexBuffer);

		commandBuffer->BindIndexBuffer(frameIndex, testIndexBuffer);

		commandBuffer->DrawIndexedInstanced(testIndexBuffer->GetBufferSize(), 1, 0, 0, 0);

		commandBuffer->BindGraphicsPipeline(frameIndex, testPipeline2);

		pos = glm::translate(glm::mat4(1.0f), glm::vec3(10.f, 0, 0));
		rot = glm::mat4_cast(glm::tquat<float>(glm::vec3(time, 0, time / 2.f)));
		scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.f));
		data.objectSpace = pos * rot * scale;

		testConstantBuffer->CopyToBuffer(frameIndex, &data, sizeof(CameraData), 1);

		commandBuffer->BindConstantBuffer(frameIndex, 0, 0, testConstantBuffer, 1);

		commandBuffer->BindImage(frameIndex, 1, 0, testImage);

		commandBuffer->BindVertexBuffer(frameIndex, testVertexBuffer);

		commandBuffer->BindIndexBuffer(frameIndex, testIndexBuffer);

		commandBuffer->DrawIndexedInstanced(testIndexBuffer->GetBufferSize(), 1, 0, 0, 0);

		commandBuffer->EndFramebuffer(frameIndex, cache->framebuffer);

		commandBuffer->CopyFramebufferToSwapchain(frameIndex, cache->framebuffer, GraphicsContext::GetSwapchain().lock());

		commandBuffer->End();

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