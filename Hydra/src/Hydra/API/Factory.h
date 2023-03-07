#pragma once
#include "Hydra/API/PhysicalDevice.h"
#include "Hydra/API/Device.h"
#include "Hydra/API/Swapchain.h"
#include "Hydra/API/Context.h"

namespace Hydra
{
	enum class API
	{
		Dx12,
		Vulkan
	};

	struct ContextConstructSpecifications
	{
		ContextSpecification contextSpecs;
		Ref<Context> context;
		API api;
	};

	class Factory
	{
	public:
		static void ContructContext(ContextConstructSpecifications specs);
	private:
	};
}