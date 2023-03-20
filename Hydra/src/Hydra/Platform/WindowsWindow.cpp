#include "HYpch.h"
#include "WindowsWindow.h"

#include "Hydra/Events/ApplicationEvent.h"
#include <Hydra/Events/InputEvent.h>


namespace Hydra
{
	static bool s_Initialized = false;
	Window* Window::Create(const WindowCreateInfo& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowCreateInfo& props)
	{
		Initialize(props);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetWindowName(const std::string& name)
	{
		glfwSetWindowTitle(m_Window, name.c_str());
	}

	void WindowsWindow::Initialize(const WindowCreateInfo& createInfo)
	{
		m_WindowInfo.Title = createInfo.Title;
		m_WindowInfo.Width = createInfo.Width;
		m_WindowInfo.Height = createInfo.Height;
		m_WindowInfo.VSync = createInfo.VSync;


		if (!s_Initialized)
		{
			int success = glfwInit();
			s_Initialized = true;
		}
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//glfwWindowHint(GLFW_DECORATED, false);
		m_Window = glfwCreateWindow(m_WindowInfo.Width, m_WindowInfo.Height, m_WindowInfo.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_WindowInfo);
		SetVSync(true);

		if (glfwRawMouseMotionSupported())
		{
			glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowInfo* info = static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
				info->Width = width;
				info->Height = height;

				WindowResizeEvent event(width, height);
				info->EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowInfo* info = static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
				WindowCloseEvent event;
				info->EventCallback(event);
			});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
		{
				static double oldX = 0;
				static double oldY = 0;
				
				WindowInfo* info = static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
				MouseMoveEvent event(xpos, ypos, xpos - oldX, ypos - oldY);
				info->EventCallback(event);
				oldX = xpos;
				oldY = ypos;
		});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
	
}
