#pragma once
#include "Hydra/Events/Event.h"
namespace Hydra
{
	class Layer
	{
	public:
		Layer(std::string name = "Layer");
		virtual void OnAttach(){}
		virtual void OnUpdate(){}
		virtual void OnDetach(){}
		virtual void OnEvent(Event& e) { e; }

		inline const std::string& GetName() const { return m_DebugName; }

		virtual ~Layer();
	protected:
		std::string m_DebugName;
	};
}
