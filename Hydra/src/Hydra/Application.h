#pragma once
#include "LayerStack.h"
#include "Hydra/Window.h"
#include <sstream>
#include "Hydra/Utils/Memory.h"

namespace Hydra
{
	class Application
	{
	public:
		Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Window& GetWindow() { return *s_Window.get(); }

		virtual ~Application(){}
	protected:
		inline static Ref<Window> s_Window;
		bool m_Running;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
}
