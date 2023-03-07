#include "HYpch.h"
#include "GraphicsContext.h"

namespace Hydra
{
	void GraphicsContext::Intitalize(API api)
	{
		ContextConstructSpecifications contextCSpecs{};
		contextCSpecs.context = s_Context;
		contextCSpecs.api = api;

		Factory::ContructContext(contextCSpecs);
	}
}