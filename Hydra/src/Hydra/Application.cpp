#include "HYpch.h"
#include "Application.h"

#include <iostream>

#include "Events/ApplicationEvent.h"
#include "Hydra/Utils/Log.h"
#include "Hydra/Rendering/GraphicsContext.h"
#include "Hydra/Rendering/Renderer.h"
#include "Hydra/Utils/Chrono.h"
Hydra::Application::Application() : m_Running(true)
{
	s_Window = Ref<Window>(Window::Create());
	s_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	Log::Initialize();
	GraphicsContext::Intitalize(API::Vulkan);
	Renderer::Initialize();
}

void Hydra::Application::Run()
{
	while (m_Running)
	{
		Chrono::UpdateTimeData();
		for (const auto& layer : m_LayerStack)
		{
			layer->OnUpdate();
		}
		s_Window->OnUpdate();
		Renderer::Begin();
	}
	Renderer::Shutdown();
	GraphicsContext::Shutdown();
}

void Hydra::Application::OnEvent(Event& e)
{
	Renderer::OnEvent(e);
	if (e.GetEventType() == EventType::WindowClose)
	{
		m_Running = false;
	}
	if (e.GetEventType() == EventType::WindowResize)
	{
		auto& resize = *reinterpret_cast<WindowResizeEvent*>(&e);
	}
	for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
	{
		(*--it)->OnEvent(e);
		if (e.Handled)
			break;
	}
}

void Hydra::Application::PushLayer(Layer* layer)
{
	m_LayerStack.Push(layer);
}

void Hydra::Application::PushOverlay(Layer* overlay)
{
	m_LayerStack.PushOverlay(overlay);
}
