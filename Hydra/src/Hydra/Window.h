#pragma once
#include "Hydra/Events/Event.h"
namespace Hydra
{
	struct WindowCreateInfo
	{
		std::string Title = "Titan Engine";
		uint32_t Width = 1280;
		uint32_t Height = 720;
		bool VSync = false;
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() = default;
		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void* GetNativeWindow() = 0;
		static Window* Create(const WindowCreateInfo& info = WindowCreateInfo());
	};
}

