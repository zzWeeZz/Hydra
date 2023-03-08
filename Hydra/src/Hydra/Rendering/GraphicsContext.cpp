#include "HYpch.h"
#include "GraphicsContext.h"

namespace Hydra
{
	void GraphicsContext::Intitalize(API api)
	{
		ContextConstructSpecifications contextCSpecs{};
		contextCSpecs.contextSpecs.queueTypes = QueueType::Graphics | QueueType::Transfer | QueueType::Compute;
		contextCSpecs.context = s_Context;
		contextCSpecs.api = api;

		Factory::ContructContext(contextCSpecs);
		s_Context = contextCSpecs.context;
	}
	void GraphicsContext::Shutdown()
	{
		s_Context->Shutdown();
		s_Context.reset();
	}
}