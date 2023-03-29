#pragma once
#include "Hydra/Events/Event.h"
namespace Hydra
{
	class Renderer
	{
	public:
		static void Initialize();
		static void OnEvent(Event& e);
		static void Begin();
		static void Shutdown();
		static void RunEditorCamera();
	};
}