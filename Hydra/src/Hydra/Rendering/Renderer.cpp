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
#include "Hydra/Utils/Chrono.h"
#include "Hydra/Utils/Input.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include "Hydra/Assets/Mesh.h"
#include <Hydra/Events/InputEvent.h>
#include "Hydra/Assets/Material.h"

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

	static MeshObject mesh;
	struct CameraData
	{
		glm::mat4 view;
		glm::mat4 proj;
		glm::mat4 objectSpace;
	};
	static CameraData camdata;
	static glm::vec3 position;
	static glm::vec3 rotation;
	static glm::vec3 scale;

	static glm::vec3 forward;

	static float m_Pitch = 0.f;
	static float m_Yaw = 0.f;
	static float m_Roll = 0.f;


	void Renderer::Initialize()
	{
		cache = std::make_shared<Cache>();
		auto device = GraphicsContext::GetDevice();
		FramebufferSpecification framebufferSpecs = {};
		framebufferSpecs.formats.emplace_back(ImageFormat::RGBA8UN);
		framebufferSpecs.formats.emplace_back(ImageFormat::Depth32);
		framebufferSpecs.width = 1280;
		framebufferSpecs.height = 720;

		device.lock()->CreateFramebuffer(framebufferSpecs, cache->framebuffer);
		ShaderCompilerCreateSpecification createSpecs = {};
		createSpecs.optimazationMode = OptimazationMode::Debug;
		createSpecs.behaviorFlag = CompilerBehaviorFlag::TreatWarningsAsErrors;
		Factory::ConstructShaderCompiler(createSpecs, shaderCompiler);

		/*void* pix;
		int texWidth, texHeight, texChannels;
		pix = stbi_load("Titan.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		ImageSpecification specs = {};
		specs.ImageData = pix;
		specs.height = texHeight;
		specs.width = texWidth;

		GraphicsContext::GetDevice().lock()->CreateImage(specs, testImage);*/


		ShaderSpecification testShaderSpecs = {};

		testShaderSpecs.vertexShaderStage.filepath = "Shaders/Triangle2_vs.hlsl";
		testShaderSpecs.pixelShaderStage.filepath = "Shaders/Triangle_ps.hlsl";
		std::string str;
		shaderCompiler->Compile(testShaderSpecs, testShader, str);

		testShaderSpecs.pixelShaderStage.filepath = "Shaders/Triangle2_ps.hlsl";
		shaderCompiler->Compile(testShaderSpecs, testShader2, str);


		GraphicsPipelineSpecification graphicsSpecs = {};

		graphicsSpecs.shaderObject = testShader2;

		graphicsSpecs.framebufferObject = cache->framebuffer;

		graphicsSpecs.bindingDescription.binding = 0;
		graphicsSpecs.bindingDescription.stride = sizeof(Vertex);

		GraphicsContext::GetDevice().lock()->CreateGraphicsPipeline(graphicsSpecs, testPipeline2);
		
		
		position = glm::vec3(0.f);
		forward = { 1, 0,0 };

		BufferCreateSpecification bufferSpecs = {};

		

		bufferSpecs.usage = BufferUsage::ConstantBuffer;
		bufferSpecs.stride = sizeof(CameraData);
		bufferSpecs.count = 40;
		bufferSpecs.data = nullptr;
		bufferSpecs.allocationUsage = MemoryUsage::CPU_To_GPU;

		GraphicsContext::GetDevice().lock()->CreateBuffer(bufferSpecs, testConstantBuffer);
		mesh.Loader("Sponza/Sponza.glb");
	}
	void Renderer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>([&] (WindowResizeEvent& e)
			{
				cache->framebuffer->Resize(e.GetWidth(), e.GetHeight());
			});

		dispatcher.Dispatch<MouseMoveEvent>([&](MouseMoveEvent& e)
			{
				if (Input::MouseButton(Mouse::ButtonRight, InputMode::Down))
				{
					float xoffset = static_cast<float>(e.GetMouseDelta().first);
					float yoffset = static_cast<float>(e.GetMouseDelta().second);


					const float sensitivity = 0.5f;
					xoffset *= sensitivity;
					yoffset *= sensitivity;

					m_Yaw += xoffset;
					m_Pitch += yoffset;

					if (m_Pitch > 89.0f)
						m_Pitch = 89.0f;
					if (m_Pitch < -89.0f)
						m_Pitch = -89.0f;

					glm::vec3 direction{};
					direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
					direction.y = sin(glm::radians(-m_Pitch));
					direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
					forward = glm::normalize(direction);
				}
				return false;
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

		RunEditorCamera();

		//camdata.view = glm::inverse(camdata.view);
		

		auto pos = glm::translate(glm::mat4(1.0f), glm::vec3());
		auto rot = glm::mat4_cast(glm::tquat<float>(glm::vec3(3.14f / 2.f, 0.f, 0.f)));
		auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.f));
		camdata.objectSpace = pos * rot * scale;

		testConstantBuffer->CopyToBuffer(frameIndex, &camdata, sizeof(CameraData), 0);

		float clear[] = { 0.32f, 0.32f, 0.32f, 1.f };

		commandBuffer->BeginFramebuffer(frameIndex, cache->framebuffer, clear);

		commandBuffer->BindGraphicsPipeline(frameIndex, testPipeline2);
		
		commandBuffer->BindConstantBuffer(frameIndex, 0, 0, testConstantBuffer, 0);


		for (auto& sub : mesh.GetSubmeshes())
		{
			auto& material = mesh.GetMaterials();
			auto tex = material[sub.materialID]->GetColorTexture().lock()->GetImage().lock();
			commandBuffer->BindImage(frameIndex, 1, 0, tex);

			commandBuffer->BindVertexBuffer(frameIndex, sub.vertexBuffer);

			commandBuffer->BindIndexBuffer(frameIndex, sub.indexBuffer);

			commandBuffer->DrawIndexedInstanced(sub.indexBuffer->GetBufferSize(), 1, 0, 0, 0);
		}


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
	void Renderer::RunEditorCamera()
	{
		float delta = Chrono::Timestep();
		if (Input::Key(Key::S, InputMode::Down))
		{
			position -= forward * delta * 10.f;
		}
		if (Input::Key(Key::W, InputMode::Down))
		{
			position += forward * delta * 10.f;
		}
		if (Input::Key(Key::D, InputMode::Down))
		{
			position += glm::cross(forward, glm::vec3(0, 1, 0)) * delta * 10.f;
		}
		if (Input::Key(Key::A, InputMode::Down))
		{
			position -= glm::cross(forward, glm::vec3(0, 1, 0)) * delta * 10.f;
		}
		if (Input::Key(Key::E, InputMode::Down))
		{
			position.y += delta * 10.f;
		}
		if (Input::Key(Key::Q, InputMode::Down))
		{
			position.y -= delta * 10.f;
		}

		camdata.view = glm::lookAt(position, position + forward, glm::vec3(0, 1, 0));
		camdata.proj = glm::perspective(glm::radians(60.f), 16.f/9.f, 0.1f, 200.f);
	}
}