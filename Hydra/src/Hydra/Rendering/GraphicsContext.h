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
	private:
		inline static Ref<Context> s_Context;
	};
}
