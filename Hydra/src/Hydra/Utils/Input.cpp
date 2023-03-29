#include "HYpch.h"
#include "Input.h"
#include "Hydra/Application.h"
#include "Hydra/Platform/WindowsWindow.h"
namespace Hydra
{
	bool Input::Key(KeyCode key, InputMode inputMode)
	{
		auto* window = (GLFWwindow*)Application::GetWindow().GetNativeWindow();
		auto state = glfwGetKey(window, (int32_t)key);
		switch (inputMode)
		{
		case Hydra::InputMode::Down:
			return state == GLFW_PRESS;
			break;
		case Hydra::InputMode::Pressed:
			return state == GLFW_REPEAT;
			break;
		case Hydra::InputMode::Up:
			return state == GLFW_RELEASE;
			break;
		default:
			break;
		}
		return false;
	}
	bool Input::MouseButton(MouseCode key, InputMode inputMode)
	{
		auto* window = (GLFWwindow*)Application::GetWindow().GetNativeWindow();
		auto state = glfwGetMouseButton(window, (int32_t)key);
		switch (inputMode)
		{
		case Hydra::InputMode::Down:
			return state == GLFW_PRESS;
			break;
		case Hydra::InputMode::Pressed:
			return state == GLFW_PRESS;
			break;
		case Hydra::InputMode::Up:
			return state == GLFW_RELEASE;
			break;
		default:
			break;
		}
		return false;
	}
}