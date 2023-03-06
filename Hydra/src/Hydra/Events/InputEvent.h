#pragma once
#include <sstream>

#include "Hydra/Events/Event.h"

namespace Hydra
{
	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(double x, double y, double deltaX, double deltaY)
		{
			m_DeltaX = deltaX;
			m_DeltaY = deltaY;
			m_X = x;
			m_Y = y;
		}

		std::pair<double, double> GetMousePos() { return { m_X, m_Y }; }
		std::pair<double, double> GetMouseDelta() { return {m_DeltaX, m_DeltaY}; }
		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryMouse)
	private:
		double m_X;
		double m_Y;
		double m_DeltaX;
		double m_DeltaY;
	};
}