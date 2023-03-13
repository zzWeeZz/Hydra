#pragma once

#include "Hydra/API/PipelineInterface/Pipeline.h"
namespace Hydra
{
	class GraphicsPipeline : public Pipeline
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	};
}