#pragma once
#include "Hydra/API/Device.h"
#include "Hydra/API/PhysicalDevice.h"
#include "Hydra/API/Swapchain.h"
namespace Hydra
{
	enum class API
	{
		Dx12,
		Vulkan
	};

	struct GraphicsContextSpecification
	{
		API api = API::Dx12;

		size_t width = 1280;
		size_t height = 720;
	};

	class GraphicsContext
	{
	public:
		static void Initalize(const GraphicsContextSpecification& specs);
	private:
		inline static Ref<Device> s_Device;
		inline static Ref<PhysicalDevice> s_PhysicalDevice;
		inline static Ref<Swapchain> s_Swapchain;
	 	inline static GraphicsContextSpecification s_Specs;
	};
}