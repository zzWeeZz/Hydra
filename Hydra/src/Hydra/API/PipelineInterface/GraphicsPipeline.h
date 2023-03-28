#pragma once

#include "Hydra/API/PipelineInterface/Pipeline.h"
#include "Hydra/API/ResourceInterface/Shader.h"
#include <Hydra/API/Framebuffer.h>
namespace Hydra
{
	struct BindAttribute
	{
		uint32_t binding = 0;
		uint32_t location = 0;
		ImageFormat format = ImageFormat::RGBA32F;
		uint32_t offset = 0;
	};

	struct VertexBindingDescription
	{
		uint32_t binding = 0;
		uint32_t stride = 0;
		VertexInputRate inputRate = VertexInputRate::InputRangeVertex;
	};

	struct GraphicsPipelineSpecification
	{
		// Shader needs to be created before creating pipeline.
		Ptr<Shader> shaderObject;

		// Framebuffer to draw on.
		Ptr<Framebuffer> framebufferObject;


		VertexBindingDescription bindingDescription;

		// width of the pipeline draw area.
		size_t width = 1280;

		// height of the pipeline draw area.
		size_t height = 720;
	};

	class GraphicsPipeline : public Pipeline
	{
	public:
		GraphicsPipeline(const GraphicsPipelineSpecification& specs) { m_Specs = specs; }

		virtual ~GraphicsPipeline(){}
	protected:
		GraphicsPipelineSpecification m_Specs = {};
	};
}