#pragma once
#include "Hydra/Utils/KeyCode.h"
#include "Hydra/Utils/MouseCode.h"
namespace Hydra
{
	enum class InputMode
	{
		Down,
		Pressed,
		Up
	};

	class Input
	{
	public:
		static bool Key(KeyCode key, InputMode inputMode);
		static bool MouseButton(MouseCode key, InputMode inputMode);
	};
}