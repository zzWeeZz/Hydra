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
	struct ShaderCompilerCreateSpecification;
	class ShaderCompiler;
	class Factory
	{
	public:
		static void ConstructContext(ContextConstructSpecifications& specs);
		static void ConstructShaderCompiler(ShaderCompilerCreateSpecification& specs, Ref<ShaderCompiler>& outShaderCompiler);
	private:
		static inline API s_API;
		static inline Ptr<Context> s_ContextHandle;
	};
}