#pragma once
#include "Hydra/API/Context.h"
#include <Hydra/API/Factory.h>
namespace Hydra
{
	class GraphicsContext
	{
	public:
		static void Intitalize(API api);
		static void Shutdown();

		static Ptr<Device> GetDevice(); 
		static Ptr<PhysicalDevice> GetPhysicalDevice(); 
		static Ptr<Swapchain> GetSwapchain(); 

	private:
		inline static Ref<Context> s_Context;
	};
}
