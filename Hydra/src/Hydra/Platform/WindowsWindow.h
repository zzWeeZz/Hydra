#pragma once
#include "Hydra/Window.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>


namespace Hydra
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowCreateInfo& props);
		~WindowsWindow() override {}
		void OnUpdate() override;

		inline uint32_t GetWidth() const override { return m_WindowInfo.Width; }
		inline uint32_t GetHeight() const override{ return m_WindowInfo.Height; }
		inline void SetVSync(bool enabled) override { m_WindowInfo.VSync = enabled; }
		inline bool IsVSync() const override {return m_WindowInfo.VSync;}
		void SetWindowName(const std::string& name) override;

		void SetEventCallback(const EventCallbackFn& callback) override { m_WindowInfo.EventCallback = callback; }
		inline void* GetNativeWindow() override { return m_Window; }
	private:
		virtual void Initialize(const WindowCreateInfo& createInfo);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;

		struct WindowInfo
		{
			std::string Title = "Titan Engine";
			uint32_t Width = 1280;
			uint32_t Height = 720;
			bool VSync = false;

			EventCallbackFn EventCallback;
		} m_WindowInfo;

		// Inherited via Window
	};
}

